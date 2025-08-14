import requests
from apscheduler.schedulers.background import BackgroundScheduler

from .config import settings
from .model import Measurement as MeasurementModel
from .orm import Measurement as MeasurementDB
from .orm import SessionMaker


def measure() -> MeasurementModel:
    response = requests.get(
        url=f'http://{settings.command_server}:{settings.command_port}/measure')
    response.raise_for_status()

    measurement = MeasurementModel.model_validate(response.json())
    session = SessionMaker()
    session.add(MeasurementDB(
        temperature=measurement.temperature,
        humidity=measurement.humidity,
        ctime=measurement.ctime
    ))
    session.commit()
    session.close()
    return measurement


scheduler = BackgroundScheduler()
scheduler.add_job(measure, 'interval', minutes=settings.measure_interval)

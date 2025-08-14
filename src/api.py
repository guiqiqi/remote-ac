import requests
from fastapi import FastAPI

from .config import settings
from .model import Aggregator, CommandExecution
from .model import Measurement as MeasurementModel
from .orm import Measurement, Command
from .orm import SessionMaker
from .task import measure

app = FastAPI()


@app.get('/')
def aggregator(limit: int = 10) -> Aggregator:
    session = SessionMaker()
    measurements = []
    for measurement in session.query(Measurement).order_by(Measurement.ctime.desc()).limit(limit):
        measurements.append(MeasurementModel.model_validate(measurement, from_attributes=True))
    command = session.query(Command).order_by(Command.ctime.desc()).first()
    if command is None:
        return Aggregator(measurements=measurements, status=False)
    else:
        return Aggregator(measurements=measurements, status=bool(command.status))


@app.post('/measure')
def manually() -> MeasurementModel:
    return measure()


@app.post('/command')
def command(power: CommandExecution) -> CommandExecution:
    if power.status:
        response = requests.get(
            f'http://{settings.command_server}:{settings.command_port}/power?status=on')
    else:
        response = requests.get(
            f'http://{settings.command_server}:{settings.command_port}/power?status=off')
    response.raise_for_status()

    session = SessionMaker()
    command = Command(status=power.status)
    session.add(command)
    session.flush()
    session.commit()
    command = session.query(Command).order_by(Command.ctime.desc()).first()
    if command:
        return CommandExecution(status=bool(command.status))
    else:
        return CommandExecution(status=False)

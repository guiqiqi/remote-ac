from sqlalchemy import create_engine, Column, Integer, DateTime, Float, Boolean
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker

from .config import settings

from datetime import datetime


Base = declarative_base()


class Measurement(Base):
    __tablename__ = 'measurements'

    id = Column(Integer, primary_key=True)
    temperature = Column(Float)
    humidity = Column(Float)
    ctime = Column(DateTime, default=datetime.now)


class Command(Base):
    __tablename__ = 'commands'

    id = Column(Integer, primary_key=True)
    status = Column(Boolean, default=False)
    ctime = Column(DateTime, default=datetime.now)


engine = create_engine(f'sqlite:///{settings.database}')
Base.metadata.create_all(engine)
SessionMaker = sessionmaker(autocommit=False, autoflush=False, bind=engine)

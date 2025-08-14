import typing as t

from pydantic import BaseModel, Field
from datetime import datetime


class Measurement(BaseModel):
    temperature: float
    humidity: float
    ctime: datetime = Field(default_factory=datetime.now)


class Aggregator(BaseModel):
    measurements: t.List[Measurement]
    status: bool


class CommandExecution(BaseModel):
    status: bool

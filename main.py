from src.config import settings
from src.task import scheduler
from src.api import router

from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles

import uvicorn


app = FastAPI()
app.include_router(router)
app.mount('/', StaticFiles(directory='static', html=True))

if __name__ == '__main__':
    scheduler.start()
    uvicorn.run(app, host=settings.listen, port=settings.port)

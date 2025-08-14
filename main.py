import uvicorn

from src.api import app
from src.task import scheduler
from src.config import settings

if __name__ == '__main__':
    scheduler.start()
    uvicorn.run(app, host=settings.listen, port=settings.port)

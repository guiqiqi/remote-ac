from pydantic_settings import BaseSettings


class Settings(BaseSettings):
    command_server: str = '192.168.50.31'
    command_port: int = 80
    measure_interval: int = 5
    database: str = 'database/remote-ac.db'
    listen: str = '0.0.0.0'
    port: int = 8888


settings = Settings()

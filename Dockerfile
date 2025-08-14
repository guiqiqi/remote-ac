FROM python:3.13-slim
RUN pip install --no-cache-dir poetry
WORKDIR /app
COPY . /app
RUN poetry config virtualenvs.in-project true && \
    poetry install --no-root --no-interaction --no-ansi
CMD ["poetry", "run", "python", "main.py"]
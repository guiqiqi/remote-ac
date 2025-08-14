## Simple Remote AC Controller

Opening AC remotely and monitoring status for my cats when on vacation.

- Hardware: ESP8266 + DHT11 temperature humidity sensor + IR
- Software: FastAPI + Docker

No authentication, since it runs locally with Tailscale LAN support.

### Usage:

Running with Docker:

```
docker build -t remote-ac .
docker run -p node.hs.local:8888:8888 -v ./database:/app/database remote-ac
```

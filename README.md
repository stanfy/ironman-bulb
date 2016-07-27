Ironman bulb project based on ESP8266
======
Python script and arduino sketch for visualization the quality of the Internet connection.

check-connection.py
------------
change next parameters:

```python
hosts =  [("provider-gateway", 3, 5), ("server-ip", 80, 4), ("foreign-public-server", 4, 1), ("public-server", 40, 2) ]

url = "http://192.168.1.5/"

timeDelay = 15 
```

esp-pwm.ino
------------
change next parameters:

```bash
const char *ssid = "your-ssid";
const char *password = "your-pass";
```




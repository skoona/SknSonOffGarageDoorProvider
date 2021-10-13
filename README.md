# SknGarageDoorProvider

<a href="https://homieiot.github.io/">
  <img src="https://homieiot.github.io/img/works-with-homie.png" alt="works with MQTT Homie">
</a>

## HomieNode V3 (ESP32) featuring:
### Node: positionService
- VL53L1x Time of Flight Sensor capturing distance reading every 280 milliseconds once triggered.
- Assumes the distance being measured is one to eight feet.
- Runs in continuous capture mode once triggered for the seconds indicated as `duration`, set in the config.

### Node: relayService
- Operates the Relay for `relayHoldTimeMS`, as set in config, when triggered.

### Node: provider
- Settable() node which controls the operation of relayService and positionService to operate Door to Open or Closed position.
- Frequent verifies door position by triggering positionService every `positionIntervalSec` seconds as indicated in config.
- Ignores OPEN or CLOSE commands if door is already in commanded positon.

### Common Public APIs:
* bool isReady()                            Is Node ready and communicating?
* void operate()                            Trigger node's action


## Homie Config: data/homie/config.json
```
{
  "name": "Garage Door Operator",
  "device_id": "GarageDoorProvider",
  "device_stats_interval": 900,  
  "wifi": {
    "ssid": "<wifi-host>",
    "password": "<wifi-password>"
  },
  "mqtt": {
    "host": "<mqtt-hostname-or-ip>",
    "port": 1883,
	"base_topic": "sknSensors/",
    "auth": true,
    "username": "<mqtt-username>",
    "password": "<mqtt-password>"
  },
  "ota": {
    "enabled": true
  },
  "settings": {
    "relayHoldTimeMS": 750,
    "positionIntervalSec": 60,
    "duration": 45
  }
}
```

## Homie Serial Runtime Log
```
20:14:15.120 >   ◦ [setHoldTimeInMilliseconds] set: 400
20:14:15.120 > 💡 Firmware Door Operator (1.0.0)
20:14:15.120 > 🔌 Booting into normal mode 🔌
20:14:15.811 > {} Stored configuration
20:14:15.814 >   • Hardware device ID: 240ac4594254
20:14:15.817 >   • Device ID: GarageDoorProvider
20:14:15.820 >   • Name: Garage Door Operator
20:14:15.823 >   • Device Stats Interval: 300 sec
20:14:15.827 >   • Wi-Fi: 
20:14:15.828 >     ◦ SSID: SFNSS1-24G
20:14:15.830 >     ◦ Password not shown
20:14:15.833 >   • MQTT: 
20:14:15.834 >     ◦ Host: openhab.skoona.net
20:14:15.837 >     ◦ Port: 1883
20:14:15.839 >     ◦ Base topic: sknSensors/
20:14:15.841 >     ◦ Auth? yes
20:14:15.843 >     ◦ Username: openhabian
20:14:15.846 >     ◦ Password not shown
20:14:15.848 >   • OTA: 
20:14:15.849 >     ◦ Enabled? yes
20:14:15.851 >   • Custom settings: 
20:14:15.853 >     ◦ relayHoldTimeMS: 750 (set)
20:14:15.856 >     ◦ positionIntervalSec: 60 (set)
20:14:15.860 >     ◦ duration: 45 (set)
20:14:15.862 > • Relay Module:
20:14:15.864 > • VL53L1x Ranging Module:
20:14:15.871 > 〽 Medium distance mode accepted.
20:14:15.875 > 〽 200us timing budget accepted.
20:14:15.878 > • Controller Module:
20:14:15.880 > ↕ Attempting to connect to Wi-Fi...
20:14:18.854 > ✔ Wi-Fi connected, IP: 10.100.1.244
20:14:18.858 > Triggering WIFI_CONNECTED event...
20:14:18.861 > ↕ Attempting to connect to MQTT...
20:14:18.908 > Sending initial information...
20:14:19.006 > ✔ MQTT ready
20:14:19.008 > Triggering MQTT_READY event...
20:14:19.011 > 〽 Node: Relay Service Ready to operate.
20:14:19.014 > 〽 Node: Ranging Service Ready to operate.
20:14:19.018 > 〽 Node: Controller Ready to operate.
20:14:19.022 > Calling setup function...
20:14:19.024 > 〽 Start continuous ranging @ 280 ms accepted.
20:14:19.028 > 〽 Sending statistics...
20:14:19.031 >   • Interval: 305s (300s including 5s grace time)
20:14:19.035 >   • Wi-Fi signal quality: 90%
20:14:19.038 >   • Uptime: 4s
20:14:19.140 > 📢 Calling broadcast handler...
20:14:19.144 > Received broadcast level alert: OH3 Offline
20:14:19.148 > 📢 Calling broadcast handler...
20:14:19.151 > Received broadcast level LWT: HomieMonitor Offline!
20:14:19.268 > 〽 range: 249 mm 	status: range valid	signal: 148.12 MCPS	ambient: 0.13 MCPS Direction: 
20:14:19.511 > 〽 range: 271 mm 	status: range valid	signal: 20.67 MCPS	ambient: 0.02 MCPS Direction: 
20:14:19.799 > 〽 range: 271 mm 	status: range valid	signal: 19.25 MCPS	ambient: 0.02 MCPS Direction: 
20:14:20.095 > 〽 range: 271 mm 	status: range valid	signal: 19.14 MCPS	ambient: 0.02 MCPS Direction: 
20:14:20.382 > 〽 range: 271 mm 	status: range valid	signal: 19.11 MCPS	ambient: 0.02 MCPS Direction: OPENING
20:14:20.652 > 〽 range: 271 mm 	status: range valid	signal: 19.15 MCPS	ambient: 0.02 MCPS Direction: OPEN
20:14:20.925 > 〽 range: 271 mm 	status: range valid	signal: 19.20 MCPS	ambient: 0.02 MCPS Direction: OPEN
20:14:21.202 > 〽 range: 270 mm 	status: range valid	signal: 19.20 MCPS	ambient: 0.02 MCPS Direction: OPEN
20:14:21.488 > 〽 range: 270 mm 	status: range valid	signal: 19.23 MCPS	ambient: 0.02 MCPS Direction: OPEN
20:14:21.766 > 〽 range: 270 mm 	status: range valid	signal: 19.19 MCPS	ambient: 0.02 MCPS Direction: OPEN
20:14:22.055 > 〽 range: 270 mm 	status: range valid	signal: 19.16 MCPS	ambient: 0.02 MCPS Direction: OPEN
20:14:22.328 > 〽 range: 270 mm 	status: range valid	signal: 19.20 MCPS	ambient: 0.01 MCPS Direction: OPEN
20:14:22.613 > 〽 range: 271 mm 	status: range valid	signal: 19.17 MCPS	ambient: 0.02 MCPS Direction: OPEN
20:14:22.876 > 〽 range: 270 mm 	status: range valid	signal: 19.27 MCPS	ambient: 0.02 MCPS Direction: OPEN
20:14:23.169 > 〽 range: 271 mm 	status: range valid	signal: 19.28 MCPS	ambient: 0.02 MCPS Direction: OPEN
20:14:23.432 > 〽 range: 270 mm 	status: range valid	signal: 19.22 MCPS	ambient: 0.02 MCPS Direction: OPEN
20:14:23.716 > 〽 range: 270 mm 	status: range valid	signal: 19.20 MCPS	ambient: 0.02 MCPS Direction: OPEN
20:14:24.004 > 〽 range: 271 mm 	status: range valid	signal: 19.20 MCPS	ambient: 0.02 MCPS Direction: OPEN
20:14:24.269 > 〽 range: 270 mm 	status: range valid	signal: 19.19 MCPS	ambient: 0.02 MCPS Direction: OPEN
```

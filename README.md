# Suxsem firmware for TS0202 ZigBee motion sensor

> EXPERIMENTAL

Heavely based on https://github.com/doctor64/tuyaZigbee

- Rock solid motion detection
- Ultra low power
- 4hrs battery and status reporting
- OTA updates (check every 12hrs)

### Known limitations
- **DO NOT** enable reporting. Device is hardcoded to report battery percentage, battery voltage and occupancy status every 4 hours. This is to optimize battery consumption and prevent additional wake-ups. (Still, this could be improved)

### Instructions

1) Flash the `motionSensor_TS0202_***.bin` file using https://github.com/Suxsem/Telink-Zigbee-serial-flash-tool **starting at address 0**

2) Copy the `converter.js` file in you *Zigbee2MQTT* configuration directory. More information here: https://www.zigbee2mqtt.io/guide/configuration/more-config-options.html#external-converters

3) Enable pairing in *Zigbee2MQTT* and long press the button on the device for more then 5 seconds, until it starts blinking

4) Ensure initial configuration is properly completed by *Zigbee2MQTT*. Specifically, check that in the *bind* tab of *Zigbee2MQTT* `PowerCfg` and `ssIasZone` clusters are binded to coordinator. If not, remove and pair the device again.
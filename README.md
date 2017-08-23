An ESP32 and ESp8266 Site-Survey-Tool

Using an ESP32 or ESP8266 in Station Mode enables received Wi-Fi signals to be displayed with
their SSID and Received Signal Strength Indicator on an OLED display.

RSSI units are relative and determined by manufacturers, so they cannot (usually) be referenced to a standard, but
an RSSI of -40dBm is strong than an RSSI of -50dBm. 

You can determine the maximum likely received signal strength by placing this site survey tool next to your Router, where
you will most likely get a reading of ~ -40dBm, next move around your property and determine where the weak signal areas are.

A strong usable signal is from -40dBm to -70dBm.

Any signal less than -70dBM (e.g. -71dBm) will lead to unreliable connects, packet loss and retries and poor link speeds.

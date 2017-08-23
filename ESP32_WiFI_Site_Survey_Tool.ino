/* 
 * An ESP32 and 1.3" OLED WiFi Signal Strength Site Survey Tool/Utility, As you move around you can determine signal strength of a WiFi Router or Extender, which is useful
 * for determining where poor Wi-Fi reception is going to be or is being encountered.
 * 
 * The 'MIT License (MIT) Copyright (c) 2017 by David Bird'. Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,  
 * distribute, sublicense, and/or sell copies of the Software and to permit persons to whom the Software is furnished to do so, subject to the following conditions:    
 * The above copyright ('as annotated') notice and this permission notice shall be included in all copies or substantial portions of the Software and where the   
 * software use is visible to an end-user.  
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER    
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF, OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * See more at http://dsbird.org.uk
*/

#include "WiFi.h"
#include "SH1106.h"        // See https://github.com/squix78/esp8266-oled-ssd1306 or via Sketch/Include Library/Manage Libraries
SH1106 display(0x3c, 5,4); // OLED display object definition (address, SDA, SCL)

void setup(){
    Serial.begin(115200);               // For serial diagnostic prints
    WiFi.mode(WIFI_STA);                // Put ESP32 into Station Mode
    Wire.begin(5,4); // (sda,scl)       // Start the Wire service for the OLED display using pin=4 for SCL and Pin-5 for SDA
    display.init();                     // Initialise the display  
    display.flipScreenVertically();     // In my case flip the screen around by 180°
    display.setContrast(255);           // If you want turn the display contrast down, 255 is maxium and 0 in minimum, in practice about 128 is OK
    display.setFont(ArialMT_Plain_10);  // Set the Font size
}

void loop(){
    byte available_networks = WiFi.scanNetworks(); // COun the number of networks being received
    if (available_networks == 0) {
        Serial.println("no networks found");
    } else
    {
      display.clear();
      for (int i = 0; i < available_networks; i = i + 1) {
        display.drawString(0,i*9,WiFi.SSID(i)+" ");       // Display SSID name
        display.drawString(110,i*9,String(WiFi.RSSI(i))); // Display RSSI for that network
        display.display();
      }
    }
    delay(1000); // Wait before scanning again
}

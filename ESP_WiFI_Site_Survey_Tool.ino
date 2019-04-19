/*
   An ESP8266 or ESP32 1.3" OLED WiFi Signal Strength Site Survey Tool/Utility.
   As you move around you can determine signal strength of a WiFi Router or Extender, which is useful
   for determining where poor Wi-Fi reception is going to be or is being encountered.
   The networks are displayed in signal strength order from highest to lowest.
   
   This software, the ideas and concepts is Copyright (c) David Bird 2019 and beyond. All rights to this software are reserved.
 
   It is prohibited to redistribute or reproduce of any part or all of the software contents in any form other than the following:
   1. You may print or download to a local hard disk extracts for your personal and non-commercial use only.
   2. You may copy the content to individual third parties for their personal use, but only if you acknowledge the author David Bird as the source of the material.
   3. You may not, except with my express written permission, distribute or commercially exploit the content.
   4. You may not transmit it or store it in any other website or other form of electronic retrieval system for commercial purposes.
   5. You MUST include all of this copyright and permission notice ('as annotated') and this shall be included in all copies or substantial portions of the software and where the software use is visible to an end-user.
 
   THE SOFTWARE IS PROVIDED "AS IS" FOR PRIVATE USE ONLY, IT IS NOT FOR COMMERCIAL USE IN WHOLE OR PART OR CONCEPT.
   FOR PERSONAL USE IT IS SUPPLIED WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
   WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
   IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN 
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
   OTHER DEALINGS IN THE SOFTWARE.

   See more at http://dsbird.org.uk
*/
#ifdef ESP8266
 #include <ESP8266WiFi.h>
 #define SDA D3
 #define SCL D4
#else
 #include <WiFi.h>
  #define SDA 5
  #define SCL 4 
#endif

#include "SH1106.h"               // See https://github.com/squix78/esp8266-oled-ssd1306 or via Sketch/Include Library/Manage Libraries
SH1106 display(0x3c, SDA, SCL);   // OLED display object definition (address, SDA, SCL)

//#include "SH1306.h"             // See https://github.com/squix78/esp8266-oled-ssd1306 or via Sketch/Include Library/Manage Libraries
//SH1306 display(0x3c, SDA, SCL); // OLED display object definition (address, SDA, SCL)

struct networks {
  String Name;
  int    Signal;
};

networks networks_received [20];
networks networks_sorted[20];

void setup() {
  Serial.begin(115200);               // For serial diagnostic prints
  WiFi.mode(WIFI_STA);                // Put ESP32 into Station Mode
  Wire.begin(D3, D4); // (sda,scl)    // Start the Wire service for the OLED display using pin=D4 for SCL and Pin-D3 for SDA
  display.init();                     // Initialise the display
  display.flipScreenVertically();     // In my case flip the screen around by 180°
  display.setContrast(255);           // If you want turn the display contrast down, 255 is maxium and 0 in minimum, in practice about 128 is OK
  display.setFont(ArialMT_Plain_10);  // Set the Font size
}

void loop() {
  byte available_networks = WiFi.scanNetworks(); // Count the number of networks being received
  if (available_networks == 0) {
    Serial.println("no networks found");
  } else
  {
    get_and_sort_networks(available_networks);
    display.clear();
    display.drawString(0, 0, "SSID"); display.drawString(102, 0, "RSSI"); display.drawLine(0, 11, 127, 11);
    for (int i = 1; i < 7; i = i + 1) {
      display.drawString(0, i * 8 + 1, networks_received[i].Name + " "); // Display SSID name
      display.drawString(110, i * 8 + 1, String(networks_received[i].Signal)); // Display RSSI for that network
      display.display();
    }
  }
  delay(200); // Wait before scanning again
}

void get_and_sort_networks(byte available_networks) {
  // First get all network names and their signal strength
  for (int i = 1; i < available_networks; i++) {
    networks_received[i].Name   = String(WiFi.SSID(i)); // Display SSID name
    networks_received[i].Signal = WiFi.RSSI(i);         // Display RSSI for that network
  }
  // Now sort in order of signal strength
  String temp_name;
  int    temp_signal, iteration = 0;
  while (iteration < available_networks) { // Now bubblesort results by RSSI
    for (int i = 2; i < available_networks; i++) {
      if (networks_received[i].Signal > networks_received[i - 1].Signal) {
        temp_name   = networks_received[i - 1].Name;
        temp_signal = networks_received[i - 1].Signal;
        networks_received[i - 1].Name  = networks_received[i].Name; // Swap around values
        networks_received[i - 1].Signal = networks_received[i].Signal;
        networks_received[i].Name   = temp_name;
        networks_received[i].Signal = temp_signal;
      }
    }
    iteration++;
  }
}

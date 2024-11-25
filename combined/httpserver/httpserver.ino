#include <ESPAsyncWebServer.h>





/* #################################################### */
/*
 *      Arduino Webserver to control LED-strip
 *      (ESPRESSIF ESP32-WROOM-32 and CHINLY WS2812B)
 * 
 *  ###################################################*/

#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
//
//#include <pthread.h>
//pthread_t modeThread;
//   

#include <ArduinoOTA.h>
#include "OTA.h"

// network credentials
#include "password.h"
char *password = WIFI_PASSWORD;
char *ssid = "Tardis";

// global flags
int newCommand = 0;

/* ## basic settings LED ## -------------------------------------------------------------------*/

#define LED_PIN       14              //data-pin to LED-Strip
#define NUM_LEDS      500             //number of leds
#define DELAYVAL      500             //delayvalue between state transitions - milliseconds



// Declare our NeoPixel strip object:
// args: number of leds, datapin, pixel type flags (gbr-bitstream, 800 KHz bitstream)
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Webserver
AsyncWebServer server(80);

// Default Color and LED-status (on/off)
bool ledStatus = false;
int red = 255, green = 165, blue = 0;
int brightness = 50;

void updateLEDs() {
  if (ledStatus) {
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(red, green, blue));
    }
    strip.setBrightness(brightness);
    strip.show();
  } else {
    // switch off
    strip.clear();
    strip.show();
  }
}

void setupServer() {
 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    // Raw-String-Literal-Syntax for simple multi-line strings
    request->send(200, "text/html", R"rawliteral(
      <!DOCTYPE html>
      <html>
      <body>
        <h1>LED Steuerung</h1>
        <label>Farbe:</label>
        <input type="color" id="colorPicker" value="#FF0000">
        <button onclick="setColor()">Farbe setzen</button>
        <br><br>
        <label>Helligkeit:</label>
        <input type="number" id="brightnessInput" value="50" min="1" max="100">
         <button onclick="setBrightness()">Helligkeit setzen</button>  
        <br><br>
        <button onclick="toggleLED()">An/Aus</button>
        <script>
          function setColor() {
            const color = document.getElementById('colorPicker').value.substring(1);
            const r = parseInt(color.substring(0,2), 16);
            const g = parseInt(color.substring(2,4), 16);
            const b = parseInt(color.substring(4,6), 16);
            fetch(`/setColor?r=${r}&g=${g}&b=${b}`);
          }
          function setBrightness() {
            const brightnessval = document.getElementById('brightnessInput').value;
            fetch(`/setBrightness?value=${brightnessval}`);
          }
          function toggleLED() {
            fetch('/toggle');
          }
        </script>
      </body>
      </html>
    )rawliteral");
  });

  // SET COLOR
  server.on("/setColor", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("r") && request->hasParam("g") && request->hasParam("b")) {
      red = request->getParam("r")->value().toInt();
      green = request->getParam("g")->value().toInt();
      blue = request->getParam("b")->value().toInt();
      updateLEDs();
    }
    request->send(200, "text/plain", "Color is set");
  });

  // SWITCH ON/OFF
  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request) {
    ledStatus = !ledStatus;
    updateLEDs();
    request->send(200, "text/plain", ledStatus ? "Eingeschaltet" : "Ausgeschaltet");
  });
    // SET BRIGHTNESS
  server.on("/setBrightness", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("value")) { // Correct parameter name
        brightness = request->getParam("value")->value().toInt();
        updateLEDs();
        request->send(200, "text/plain", "Helligkeit gesetzt");
    } else {
        request->send(400, "text/plain", "Fehlender Parameter: value");
    }
});
}

void setup() {
  // Connect to WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // initialize OTA:
   setupOTA("LED_esp32", "meow");
  
  // Initialize LED-Strip
  strip.begin();
  strip.show();

  // Start Webserver
  setupServer();
  server.begin();
}

void loop() {
  /* can be called multiple times - needs to be called frequently */
  ArduinoOTA.handle();
}

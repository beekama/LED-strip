


/* #################################################### */
/*
 *      Arduino Webserver to control LED-strip
 *      (ESPRESSIF ESP32-WROOM-32 and CHINLY WS2812B)
 * 
 *  ###################################################*/

#include <string.h> 
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


/* ## basic settings LED ## */

#define LED_PIN         2               //data-pin to LED-Strip
#define LED_COUNT     300             //number of leds
#define DELAYVAL      500                  //delayvalue between state transitions

// Declare our NeoPixel strip object:
// args: number of leds, datapin, pixel type flags (gbr-bitstream, 800 KHz bitstream)
Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


/* ## basic settings SERVER ## */

// network credentials
const char* ssid = "wlan-antenne";
const char* password = "Dx%3am2?PqZ=1";

// create server that listens on port @parameter:
WiFiServer server(80);

// Variable to store the HTTP request
String client_message;

// Auxiliar variables to store the current output state
String output26State = "off";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

int red, green, blue;


/* ## setup() -- runs once at startup ## */
void setup() 
{
  // initialize serial interface:
   Serial.begin(115200);
   Serial.print("Connecting to ");
   Serial.println(ssid);
   //connecting to wifi network:
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED)
   {
      delay(500);
      Serial.print(".");
   }
   //print ip-address and start server:
   Serial.print("Interface at (ip-address): ");
   Serial.println(WiFi.localIP());
   server.begin();

   //initialize neopixel-strip object:
   pixels.begin();
}




/* ## loop() -- loops continuously as long is bord is running ## */
void loop(){
  
  /* set pixel-value to 0 */
  pixels.clear();

  /* listen for incoming clients */
  WiFiClient client = server.available();
  
  if (client){
    if (client.connected()){
      
      int BUFFER_LEN = 2000;
      char buffer[BUFFER_LEN];
      read_http(buffer, BUFFER_LEN, client);
      Serial.println(buffer);
      
      send_http_ok(client);

      char data[BUFFER_LEN];
      extract_data(buffer, data);

      if(strlen(data) > 0 && data[0] == 'A'){
        mode_1();
      }else{
        mode_2();
      }
      
    }
    client.stop();
  }      
}

void extract_data(char* httpFull, char* extractedDataBuffer){
  char* token = strtok(httpFull, "\n");

  /* skip header */
  while(token && strlen(token) > 1){
    token = strtok (NULL, "\n");
    Serial.println(token);
    Serial.println(strlen(token));
  }

  /* next line is data */
  token = strtok (NULL, "\n");
  Serial.println(token);
  
  strcpy(extractedDataBuffer, token);
  Serial.println(extractedDataBuffer);
}

  void send_http_ok(WiFiClient client){
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: text/html");
    client.println("Connection: close");
    client.println();
    client.println();
  }          

 void set_led_color(int red, int green, int blue){
            //set pixels to given value:
          for ( int i = 0; i < LED_COUNT ; i++ )
          {
            pixels.setPixelColor(i, pixels.Color( red, green, blue ));
            pixels.show();
            //delay(DELAYVAL);
          }
 }          

 void read_http(char* buffer, int bufLen, WiFiClient client){
    int count = 0;
    bool lineBlank = true;
    char c = -1;
    do {
      if(count >= bufLen - 1){
        count++;
        break;
      }
      if (client.available()){
        c = client.read();
        buffer[count] = c;
      }
      count++;
    }while (c);
    Serial.print(count);
    buffer[count] = '\0';
 }
      

void mode_1(){
  set_led_color(255,0,0);
}

void mode_2(){
  set_led_color(0,255,0);
}

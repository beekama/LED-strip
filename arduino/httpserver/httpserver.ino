


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

// network credentials
#include "password.h"
const char *password = WIFI_PASSWORD;
const char* ssid = "wlan-antenne";

/* ## basic settings LED ## */

#define LED_PIN         2               //data-pin to LED-Strip
#define LED_COUNT     300             //number of leds
#define DELAYVAL      500                  //delayvalue between state transitions



// Declare our NeoPixel strip object:
// args: number of leds, datapin, pixel type flags (gbr-bitstream, 800 KHz bitstream)
Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


/* ## basic settings SERVER ## */



// create server that listens on port @parameter:
WiFiServer server(80);

// Variable to store the HTTP request
String client_message;

// Auxiliar variables to store the current output state
String output26State = "off";

int red, green, blue;


//set time limit in case clients stop responding:
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;


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
    currentTime = millis();
    previousTime = currentTime;
    if (client.connected() && currentTime - previousTime <= timeoutTime){
      int BUFFER_LEN = 2000;
      char buffer[BUFFER_LEN];
      read_http(buffer, BUFFER_LEN, client);
      Serial.println(buffer);
      
      send_http_ok(client);

      char data[BUFFER_LEN];
      extract_data(buffer, data);

      //await rgb-values:
      //rgbRRRGGGBBB
      if(strlen(data) > 0 && data[0] == 'r' && data[1] == 'g' && data[2] == 'b'){
        int red = ASCII_to_Int(data[3])*100 + ASCII_to_Int(data[4])*10 + ASCII_to_Int(data[5]);
        int green = ASCII_to_Int(data[6])*100 + ASCII_to_Int(data[7])*10 + ASCII_to_Int(data[8]);
        int blue = ASCII_to_Int(data[9])*100 + ASCII_to_Int(data[10]) + ASCII_to_Int(data[11]);
        Serial.print("[3] ");
        Serial.print(data[3]);
        Serial.print(" - ");
        Serial.println(int(data[3]));
        Serial.print("red");
        Serial.println(red);
        Serial.print("green");
        Serial.println(green);
        Serial.print("blue");
        Serial.println(blue);                
        set_led_color(red, green, blue);
      }

      //await mode:
      if(strlen(data) > 0 && data[0] == '0'){
        mode_0();
      }else if (strlen(data) > 0 && data[0] == '1'){
        //wooop();
        mode_nature();
      }
      
    }
    client.stop();
  }      
}

/* parse ASCII-character to Integer */
int ASCII_to_Int(char c){
  switch (int(c)){
    case 48: return 0;
    case 49: return 1;
    case 50: return 2;
    case 51: return 3;
    case 52: return 4;
    case 53: return 5;
    case 54: return 6;
    case 55: return 7;
    case 56: return 8;
    case 57: return 9;
    default: return 0;
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
      

void mode_0(){
  set_led_color(0,0,0);
}

void mode_1(){
  set_led_color(255,0,0);
}

void mode_2(){
  set_led_color(0,255,0);
}

void mode_nature(){
  int color[6][3] = {{153,255,204},{178,255,102},{0,204,102},{128,255,0},{0,255,128},{200,0,0}};
  for (int w=0;w<250;w+=50) {
    for (int i=0; i<300;i++){
     pixels.setPixelColor(i,pixels.Color(color[w][0],color[w][1],color[w][2]));
      pixels.show();
      delay(100);
    }
    delay(5000);
  }
}

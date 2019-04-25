#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<ESP8266mDNS.h>
#include"SSD1306.h"
#include<Adafruit_NeoPixel.h>


const char* ssid="TP-LINK_8E8C48";
const char* password="1393heliya";
int PIN=14;
int NUM_LEDS=7;

ESP8266WebServer server(80);

SSD1306 display(0x3c,4,5);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS,PIN,NEO_GRB+NEO_KHZ800); 


void setup(){
  strip.begin();
  //...............................................
display.init();
display.flipScreenVertically();
display.setFont(ArialMT_Plain_16);
display.setTextAlignment(TEXT_ALIGN_LEFT);

//--------------------------------------------------------
pinMode ( 2,OUTPUT );
digitalWrite(2,1);
pinMode(16,OUTPUT);
digitalWrite(16,1);
//.....................................................
Serial.begin(115200);
WiFi.disconnect();
WiFi.begin(ssid,password);
while (WiFi.status()!= WL_CONNECTED )
{
  digitalWrite(2,0);
  delay(250);
  Serial.print(".");
  digitalWrite(2,1);
  delay(250);
}

Serial.print("Connected!!,IP Address:"+ WiFi.localIP().toString());
display.drawString(0,0,"IP Address:\n"+ WiFi.localIP().toString());
display.display();
//........................................................
server.begin();
Serial.println("HTTP Server Started");
server.on("/",Handle_Root);
server.on("/RGB",Handle_RGB);

}


void loop() {
 server.handleClient();

}

void Handle_Root(){
  server.send(200,"text/plain","Test RGB LEDS");
   
}

void Handle_RGB(){
  String RED = server.arg("Red");
  String GREEN = server.arg("Green");
  String BLUE = server.arg("Blue");
  String LED_NUMBER=server.arg("Led_number");
  String ALL = server.arg("All");

  if (ALL=="on"){
    for(uint16_t i=0 ; i<NUM_LEDS ; i++)
     strip.setPixelColor(i,RED.toInt(),GREEN.toInt(),BLUE.toInt());
  strip.show();
  delay(1);

  display.clear();
  display.drawString(0,0,"Red= "+ RED);
  display.drawString(0,17,"Green= "+ GREEN);
  display.drawString(0,33,"Blue= "+ BLUE);
  display.drawString(0,48,"All LEDS");
  display.display();
  }
else{
  strip.setPixelColor(LED_NUMBER.toInt(),RED.toInt(),GREEN.toInt(),BLUE.toInt());
  strip.show();
  display.clear();
  display.drawString(0,0,"Red= "+RED);
  display.drawString(0,17,"Green= "+GREEN);
  display.drawString(0,33,"Blue= "+BLUE);
  display.drawString(0,48,"Led_number: " +LED_NUMBER );
  display.display();
}

}


#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include "MyTypes.h"
#include "DigitalIO.h"
#include <TouchScreen.h>

const uint64_t pipes[2] = { RADIO_SEND_PIPE,RADIO_RECEIVE_PIPE  };
int curTemperature = 0;
int targetTemperature = 0;
bool targetTempDirty = true;

RF24 radio(RADIO_EN,RADIO_CS); // Doublecheck your wiring. Troubleshooting pain be ahead.
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Button onButton{140,10, 60, 60,YELLOW};
Button offButton{140,100, 60, 60,YELLOW};

void setup() {
  Serial.begin(57600);
  printf_begin();
  printf("=========== Loading ===========");
  tft.reset();
  tft.begin(tft.readID());
  tft.fillScreen(BLACK);
  radio.begin();
  
  // Set the proper comm channels
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
  radio.printDetails();
  
  
}
void sendTargetTemperature(int targetTemperature) {
  if(targetTemperature != NULL || targetTemperature != 0){
       uint8_t data[32];  // we'll transmit a 32 byte packet
       data[0] = targetTemperature; // First byte in the packet
       Serial.println(data[0]);
       radio.stopListening();
       radio.write(&data, sizeof(data));
       radio.startListening(); 
   }
}
void animateButtonPress(Button button){
  tft.fillRect(button.x,button.y,button.w,button.h,button.color);
       delay(500); 
       tft.fillRect(button.x,button.y,button.w,button.h,BLACK);
}
bool detectButtonPress(Button newButton,int x,int y) {
  if(x > newButton.x && (x < (newButton.x + newButton.w)) && (y > newButton.y && (y < newButton.y + newButton.h))) {
   return true;
}
  else { return false;}
}

void loop() {
  // Draw interface
  tft.drawRect(onButton.x,onButton.y,onButton.w,onButton.h,onButton.color);
  tft.drawRect(offButton.x,offButton.y,offButton.w,offButton.h,offButton.color);  
  // Get touchscreen location
  TSPoint p = ts.getPoint();
  // Flip pins back for screen display (don't ask)
   pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  p.x = tft.width()-(map(p.x, TS_MINX, TS_MAXX, tft.width(), 0));
  p.y = tft.height()-(map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
  if (p.z >= MINPRESSURE && p.z <= MAXPRESSURE) {
     // Do things with touch here
     // Detect ON
     if(detectButtonPress(onButton,p.x,p.y)) {
       Serial.println("ON pressed");
       targetTemperature++;
       animateButtonPress(onButton);
       targetTempDirty = true;
      
     }
     if(detectButtonPress(offButton,p.x,p.y)) {
       Serial.println("OFF pressed");
       targetTemperature--;
       animateButtonPress(offButton);
       targetTempDirty = true;
     }
  }
  // Update screen
  if(targetTempDirty){
      tft.fillRect(50,50,70,45,BLACK);
      tft.setCursor(50, 50);
      tft.setTextColor(BLUE);    tft.setTextSize(6);
      tft.println(targetTemperature,DEC);
      
      targetTempDirty = false;
  }
  sendTargetTemperature(targetTemperature);
  // Listen for the temperature
  
  if(radio.available())
  { 
    uint8_t rx_data[32];  // we'll receive a 32 byte packet
    radio.read( &rx_data, sizeof(rx_data) );
   
    if(curTemperature != rx_data[1]){ //Update only on change
      curTemperature = rx_data[1];
      tft.fillRect(100,280,60,40,BLACK);
      tft.setCursor(100, 280);
      tft.setTextColor(BLUE);    tft.setTextSize(5);
      tft.println(curTemperature,DEC);
      
    }
    if(targetTemperature == 0) {targetTemperature = curTemperature; targetTempDirty=true;}
  }
   printf("REMOTE: temperature is: %d degrees\n\r",curTemperature);
   
    
}



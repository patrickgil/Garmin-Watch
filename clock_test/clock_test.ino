/**************************************************************************
  This is a library for several Adafruit displays based on ST77* drivers.

  Works with the Adafruit 1.8" TFT Breakout w/SD card
    ----> http://www.adafruit.com/products/358
  The 1.8" TFT shield
    ----> https://www.adafruit.com/product/802
  The 1.44" TFT breakout
    ----> https://www.adafruit.com/product/2088
  The 1.3" TFT breakout
  ----> https://www.adafruit.com/product/4313
  The 1.54" TFT breakout
    ----> https://www.adafruit.com/product/3787
  The 2.0" TFT breakout
    ----> https://www.adafruit.com/product/4311
  as well as Adafruit raw 1.8" TFT display
    ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams.
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional).

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 **************************************************************************/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <TimeLib.h>
#include <Coordinates.h>
Coordinates point = Coordinates();

#define leftbutton 25
#define middlebutton 26
#define rightbutton 27

#define TFT_CS 5
#define TFT_RST 22
#define TFT_DC 21

#define xOffset 64
#define yOffset 80
//
// define not needed for all pins; reference for ESP32 physical pins connections to VSPI:
// SDA  GPIO23 aka VSPI MOSI
// SCLK GPIO18 aka SCK aka VSPI SCK
// D/C  GPIO21 aka A0 (also I2C SDA)
// RST  GPIO22 aka RESET (also I2C SCL)
// CS   GPIO5  aka chip select
// LED  3.3V
// VCC  5V
// GND - GND
//

// OPTION 1 (recommended) is to use the HARDWARE SPI pins, which are unique
// to each board and not reassignable. For Arduino Uno: MOSI = pin 11 and
// SCLK = pin 13. This is the fastest mode of operation and is required if
// using the breakout board's microSD card.

// For 1.44" and 1.8" TFT with ST7735 use:
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// For 1.3", 1.54", and 2.0" TFT with ST7789:
//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// OPTION 2 lets you interface the display using ANY TWO or THREE PINS,
// tradeoff being that performance is not as fast as hardware SPI above.
//#define TFT_MOSI 11  // Data out
//#define TFT_SCLK 13  // Clock out

// For ST7735-based displays, we will use this call
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// OR for the ST7789-based displays, we will use this call
//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

float p = 3.1415926;

void setup(void)
{
  Serial.begin(9600);
  Serial.print(F("Hello! 2019 IEE Garmin Smart Watch..."));
  pinMode(leftbutton, INPUT);
  digitalWrite(leftbutton, HIGH);
  pinMode(middlebutton, INPUT);
  digitalWrite(middlebutton, HIGH);
  pinMode(rightbutton, INPUT);
  digitalWrite(rightbutton, HIGH);
  // OR use this initializer (uncomment) if using a 1.44" TFT:
  tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab
  Serial.println(F("Initializing display..."));

  //I think this prints operation time?
  uint16_t time = millis();
  tft.fillScreen(ST77XX_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);

  //Display functions can go here:
  testdrawtext("garmin@watch:~ $ now", ST77XX_WHITE);
  delay(500);



  Serial.println("Done.");
  delay(1000);
  
  //setTime(hr,min,sec,day,mnth,yr);
  setTime(18,59,0,12,11,2019);


}
int prevSecond = second();
void loop()
{
//tftPrintTest();
  displayButton();
  if (second() == (prevSecond + 1 == 60 ? 0 : prevSecond + 1))
  {
    displayAnalog();
    prevSecond = second();
  }
  //displayDigital();
//  tft.invertDisplay(true);
//  delay(500);
//  tft.invertDisplay(false);
//  delay(500);
}

void testdrawtext(char *text, uint16_t color)
{
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.println(text);
}

void testdrawrects(uint16_t color)
{
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x = 0; x < tft.width(); x += 6)
  {
    tft.drawRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2, x, x, color);
  }
}

//Analog Clock -- NEEDS WORK...
float secPhi = second()*6*PI/180;
float minPhi = minute()*6*PI/180;
float hourPhi = hourFormat12()*30*PI/180;
void displayAnalog(){
  tft.drawLine(xOffset,yOffset, xcart(22,hourPhi), ycart(22, hourPhi), 0x0000);
  tft.drawLine(xOffset,yOffset, xcart(44,minPhi), ycart(44, minPhi), 0x0000);
  tft.drawLine(xOffset,yOffset, xcart(44,secPhi), ycart(44, secPhi), 0x0000);
  tft.drawCircle(xOffset, yOffset, 45, 0xFFFF);

  secPhi = second()*6*PI/180;
  minPhi = minute()*6*PI/180;
  hourPhi = hourFormat12()*30*PI/180;

  for (int i = 0; i < 12; i ++)
  {
    float phi = i * 30 * PI / 180;
    tft.drawLine(xcart(42,phi), ycart(42, phi), xcart(45, phi), ycart(45, phi), 0xFFFF);
  }
  
  tft.drawLine(xOffset,yOffset, xcart(22,hourPhi), ycart(22, hourPhi), 0xFFFF);
  tft.drawLine(xOffset,yOffset, xcart(44,minPhi), ycart(44, minPhi), 0xFFFF);
  tft.drawLine(xOffset,yOffset, xcart(44,secPhi), ycart(44, secPhi), 0xF000);
}

double xcart(int r, double phi) {
  return(r*sin(phi)+xOffset);  
}
double ycart(int r, double phi) {
  return(-r*cos(phi)+yOffset);  
}

void displayDigital() {
  displayTime();
  displayDate();
  displayBatt();
}

void displayButton() {
    //testing button press
  if (digitalRead(leftbutton)==1) {
    tft.setCursor(0,25);
    tft.setTextColor(0x0418, ST77XX_BLACK);
    tft.print("ON");
    }
  else {
    tft.setCursor(0,25);
    tft.setTextColor(0x0418, ST77XX_BLACK);
    tft.print("OFF");
  }
  if (digitalRead(middlebutton)==1) {
    tft.setCursor(0,50);
    tft.setTextColor(0x0418, ST77XX_BLACK);
    tft.print("ON");
    }
  else {
    tft.setCursor(0,50);
    tft.setTextColor(0x0418, ST77XX_BLACK);
    tft.print("OFF");
  }
  if (digitalRead(rightbutton)==1) {
    tft.setCursor(0,75);
    tft.setTextColor(0x0418, ST77XX_BLACK);
    tft.print("ON");
    }
  else {
    tft.setCursor(0,75);
    tft.setTextColor(0x0418, ST77XX_BLACK);
    tft.print("OFF");
  }
}

void tftPrintTest()
{
  tft.setCursor(0,0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextWrap(false);
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(ST77XX_MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(" seconds.");
}

void displayTime(){
  tft.setCursor(0,0);
  //tft.fillScreen(ST77XX_BLACK); commenting out to avoid obvious refresh artifact
  tft.setTextWrap(false);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setCursor(0,24);
  tft.print("[TIME] ");
  tft.setTextColor(0xFADF, ST77XX_BLACK);
  if(hour() < 10){
      tft.print("0");
    }
  tft.print(hour());
  tft.print(":");
  if(minute() < 10){
      tft.print("0");
    }
  tft.print(minute());
  tft.print(":");
  if(second() < 10){
    tft.print("0");
  }
  tft.print(second());
  delay(500);
  }

  void displayDate(){
    tft.setCursor(0,34);
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.print("[DATE] ");
    tft.setTextColor(0x0418, ST77XX_BLACK);
    tft.print(day());
    tft.print("/");
    tft.print(month());
    tft.print("/");
    tft.print(year());
    }

void displayBatt(){
  tft.setCursor(0,44);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print("[BATT] ");
  tft.setTextColor(0x0410, ST77XX_BLACK);
  tft.print("[########..]");
  }

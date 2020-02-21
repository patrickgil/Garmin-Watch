/*
 * file DFRobot_ST7687S_Latch.ino
 *
 * @ https://github.com/DFRobot/DFRobot_ST7687S
 *
 * connect with your board (please reference board compatibility)
 * 
 * controler (uno)        DFRobot 2.2 inch tft lcd display
 * SPI_SCK         ---->  SPI_SCL
 * SPI_MOSI        ---->  SPI_MOSI
 * 3               ---->  cs
 * 5               ---->  rs
 * 6               ---->  wr
 * 7               ---->  lck
 * 
 * show many graphics on board
 *
 * Copyright   [DFRobot](http://www.dfrobot.com), 2016
 * Copyright   GNU Lesser General Public License
 *
 * version  V1.0
 * date  2017-12-7
 */

#include "DFRobot_ST7687S_Latch.h"

//uint8_t pin_cs = 17, pin_rs = 21, pin_wr = 16, pin_lck = 15;

#define pin_cs 17
#define pin_rs 21
#define pin_wr 16
#define pin_lck 15

DFRobot_ST7687S_Latch tft(pin_cs, pin_rs, pin_wr, pin_lck);

void setup(void)
{
  Serial.begin(115200);
  tft.begin();
  tft.fillScreen(DISPLAY_WHITE);
}

void loop(void)
{
  tft.drawRect(-20, -20, 40, 40, DISPLAY_BLUE);  //draw rectangle
  delay(1000);
  tft.fillRect(-20, -20, 40, 40, DISPLAY_BLUE);  //fill rectangle
  delay(1000);
  tft.fillScreen(DISPLAY_WHITE);
  delay(1000);
}

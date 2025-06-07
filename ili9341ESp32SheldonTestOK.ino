/* Voltiverso 
 ESP32 & ILI9341 Display Test Sheldon attack v1.0  ManuUniverso 2025

Simple test to connect an ESP32 DevKit V1 Node32s to a TFT Touch ILI9341 display with three alternative images. 
When pressing the display, the images alternate, creating a basic animation to verify functionality.

========================================================================================================
======================= OPEN SOURCE LICENSE ============================================================
========================================================================================================

Created and designed by ManuUniverso

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
including, the rights to use, copy, modify, merge, publish, distribute, sublicense, and allow persons to whom the Software is provided to do so, subject to the following conditions:

The above copyright notice and this permission notice will be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING OUT OF, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS OF THE SOFTWARE.

............................................................................................................
Release history
............................................................................................................
Version Date Author Comment
1.0.0. 01/06/2025 First tested OK

*/

#include "GraficSC.h"                                     // Imagen para las animaciones
#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include <XPT2046_Touchscreen.h>

#define SerialRate 38400
#define TFT_DC 2                                           /// Display ili9341 pin para imagen
#define TFT_CS 15                                          /// Display ili9341 pin para imagen
#define TFT_MOSI 23                                        /// Display ili9341 pin para imagen (no se especifica en "Adafruit_ILI9341 tft" para evitar el [SPI Software limitado a 400Khz Max]; es mejor [SPI Material integrada puede alcanzar Mhz]
#define TFT_CLK 18                                         /// Display ili9341 pin para imagen  
#define TFT_RST 4                                          /// Display ili9341 pin para imagen
#define TFT_MISO 19                                        /// Display ili9341 pin para imagen
#define TCS_PIN  21                                        /// Display ili9341 pin para Touch GPIO35
#define TIRQ_PIN  34                                       /// Display ili9341 pin para Touch GPIO34 [pendiente probar no usado especificamente en esta version]

int MenaceXStart =120;
int MenaceXEnd =130;
int MenaceYStart =140;
int MenaceYEnd =150;
int MenaceRad = 7;
int MenaceMovSpeed = 2;

int BodyX = 80;
int BodyY = 80;

String BodyStat ="Pause";
String BodyStatNext ="AtackDedo"; 

uint16_t TchX=0;                     /// Menu eje X Colocacion de background para el menu solicitado
uint16_t TchY=0;                     /// Menu eje Y ...

uint16_t TchXmin=10;                 /// Menu eje Y ...
uint16_t TchXmax=6000;               /// Menu eje X Colocacion de background para el menu solicitado
uint16_t TchYmin=10;
uint16_t TchYmax=6000;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC,TFT_RST);  /// Display ili9341 pin IMAGEN
XPT2046_Touchscreen ts(TCS_PIN, TIRQ_PIN);                         /// Display ili9341 pin TOUCH   Param 2 - Touch IRQ Pin - interrupt enabled polling

void SerialSet(){        
  Serial.begin(SerialRate);       // Initialiser le port série
}
void PinSet(){            // initial setup pin esp32
  pinMode(TCS_PIN,OUTPUT);       // Asegurarse que el pin CS este a nivel HIGH para iniciar
  digitalWrite(TCS_PIN,HIGH);    //    "
  pinMode(TFT_CS,OUTPUT);
  digitalWrite(TFT_CS,HIGH);
}
void DisplaySet(){        // initial setup display
  tft.begin(40000000);        // can be raised to tft.begin(40000000) to avoid flickering   tft.begin(40000000); 
  tft.setRotation(3);         // rotate screen  0 1 2 3  =  0º 90º 180º 270º
  ts.begin();                 // activate the touch function of the display
  ts.setRotation(3);          // rotate screen touch should be the same as the rotated image (tft.setRotation)
  tft.fillScreen(ILI9341_WHITE);
}
void TouchAsk(){       // Touch Did I touch a button on the screen?
  if (ts.touched()) {            // screen touched?
    TS_Point p = ts.getPoint();  // save point
    TchX=p.x;                    // save point axis X
    TchY=p.y;                    // save point axis Y
    if(BodyStat=="Pause"){
      if ((TchX > TchXmin && TchX < TchXmax) && (TchY > TchYmin && TchY < TchYmax)) {
        BodyStatNext="Estri";
      }
    }else if(BodyStat=="Estri"){
      if ((TchX > TchXmin && TchX < TchXmax) && (TchY > TchYmin && TchY < TchYmax)) {
        BodyStatNext="AtackDedo";
      }
    }
    }else if(BodyStat=="AtackDedo"){
      if ((TchX > TchXmin && TchX < TchXmax) && (TchY > TchYmin && TchY < TchYmax)) {
        BodyStatNext="Pause";
      }
    }
    if(BodyStatNext!=BodyStat){  
      Touched();
      delay(15);
    }
    
}
void Touched(){       
    if(BodyStatNext=="Pause"){
      GrafPausePrint();
      BodyStat="Pause";
    }else if(BodyStatNext=="AtackDedo"){
      GrafAtackDedoPrint();
      BodyStat="AtackDedo";
   }else if(BodyStatNext=="Estri"){
      GrafEstriPrint();
      BodyStat="Estri";
  }
}
void GrafPausePrint(){  
  tft.drawRGBBitmap(BodyX, BodyY, GrafPause, GrafWIDTH,GrafHEIGHT); 
}

void GrafAtackDedoPrint(){  
  tft.drawRGBBitmap(BodyX, BodyY, GrafAtackDedo, GrafWIDTH,GrafHEIGHT);  
}

void GrafEstriPrint(){
  tft.drawRGBBitmap(BodyX, BodyY, GrafEstri, GrafWIDTH,GrafHEIGHT);  
}

void setup() {
  SerialSet();
  while (!Serial && (millis() <= 1000));    // pause 1 seg
  PinSet();
  DisplaySet();
}

void loop() {
  TouchAsk();
}

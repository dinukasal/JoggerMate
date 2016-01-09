/*
 * JoggerMate 2.0
 * 
 * Charith Eranga
 * Dinuka Salwathura
 */

#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 

int drawState;
int t;

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  
  switch(drawState){
    case 0:
      u8g.drawStr( 35, 13, "Welcome!");
      u8g.drawStr( 10, 35, "JoggerMate 2.0");
    break;

    case 1:
      u8g.drawFrame(110,0,15,7);
      u8g.drawBox(117,0,7,7);
      u8g.drawStr( 20, 10, "JoggerMate");
      u8g.drawTriangle();
      break;
      
    case 2:
    break;
  }
}

void setup() {
  // put your setup code here, to run once:
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  drawState=0;
}

void loop() {
  // put your main code here, to run repeatedly:
  t=millis();
  while(1){
    u8g.firstPage();
    do {
      draw();
    } while( u8g.nextPage() );
    if((millis()-t)>1500){
      drawState=1;
    }
  }
}


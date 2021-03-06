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

      break;
      
    case 2:
      u8g.drawFrame(110,0,15,7);
      u8g.drawBox(117,0,7,7);
      u8g.drawStr( 20, 10, "JoggerMate");
      //left arrow
      u8g.drawTriangle(0,35,5,40,5,30);
      u8g.drawBox(5,33,8,4);
      //right arrow
      u8g.drawTriangle(45,35,39,40,39,30);
      u8g.drawBox(31,33,8,4);
      
      u8g.drawStr( 5, 55, "1");
      u8g.drawStr( 30, 55, "2");

      //progress bars
      u8g.drawStr( 57, 35, "1 ||||||||||");
      u8g.drawStr( 57, 55, "2 |||");
    break;

    case 3:
      u8g.drawFrame(110,0,15,7);
      u8g.drawBox(117,0,7,7);
      u8g.drawStr( 20, 10, "JoggerMate");
      u8g.setFont(u8g_font_profont11);

      // left arrow
      u8g.drawTriangle(0,50,5,55,5,45);
      u8g.drawBox(5,48,8,4);
      // right arrow
      u8g.drawTriangle(45,50,39,55,39,45);
      u8g.drawBox(31,48,8,4);
      // up arrow
      u8g.drawTriangle(22,23,17,30,27,30); 
      u8g.drawBox(20,30,4,8);
      u8g.drawStr( 9, 34, "1");
      u8g.drawStr( 5, 64, "2");
      u8g.drawStr( 30, 64, "3");

      //progress bars
      u8g.drawStr( 57, 30, "1 |||||");
      u8g.drawStr( 57, 45, "2 ||||||||||");
      u8g.drawStr( 57, 60, "3 |||");
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
      t=millis();
      if(drawState==2){
        drawState=3;  
      }else{
        drawState=2;
      }
      
    }
  }
}



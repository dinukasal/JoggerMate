/*
 * JoggerMate 2.0
 * 
 * Charith Eranga
 * Dinuka Salwathura
 */

#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 35, 12, "Welcome!");
  u8g.drawStr( 10, 35, "JoggerMate 2.0");
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
  welcome();
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  displayData();
  
}
void welcome(){
   u8g.firstPage();
  do {
    draw();
    //u8g.drawBitmapP( 0, 0, 1, 8, background);
    //
  } while( u8g.nextPage() );
}

void displayData(){
  u8g.firstPage();
  
  u8g.setCursorFont(u8g_font_cursor);
  u8g.setCursorPos(20,40);
  u8g.setCursorStyle(143);
  u8g.enableCursor();
  /*
//35,101
  u8g.setCursorPos(30,40);
  u8g.setCursorStyle(145);
  u8g.enableCursor();
  */
  
  do{
    u8g.drawFrame(110,0,15,7);
    u8g.drawBox(117,0,7,7);
    u8g.drawStr( 20, 10, "JoggerMate");
    u8g.drawStr( 55, 30, "1 |||");
    u8g.drawStr( 55, 50, "2 |");
  }while(u8g.nextPage());  
  
}


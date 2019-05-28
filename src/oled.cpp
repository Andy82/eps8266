#include "SSD1306Wire.h"

#define SDA 5  
#define SCL 4 

SSD1306Wire  display(0x3c, SDA, SCL);

void Oled_init()
{

  display.init();
  display.flipScreenVertically();
  display.setContrast(255);
  // Align text vertical/horizontal center
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_10);
  display.drawString(display.getWidth()/2, display.getHeight()/2, "Ready for OTA 112:\n");
  //display.drawString(1, 1, "Ready for OTA 111:\n");
  display.display();
}
#include "display.h"

namespace{
  Adafruit_SSD1306* display;
}

void initDisplay()
{
  delay(500);
  display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed");
    for(;;); // Don't proceed, loop forever
  }

  Serial.println("OLED Display initiating!");
  // Display logo
  display->display();
  delay(2000);

  // Clear the buffer
  display->clearDisplay();
  display->display();
}

void displayText(String msg)
{
  const char* buffer = msg.c_str();
  display->clearDisplay();

  display->setTextSize(1);      // Normal 1:1 pixel scale
  display->setTextColor(SSD1306_WHITE); // Draw white text
  display->setCursor(0, 0);     // Start at top-left corner
  display->cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<msg.length(); i++) {
    if(i == '\n') display->write(' ');
    else          display->write(buffer[i]);
  }

  display->display();
}
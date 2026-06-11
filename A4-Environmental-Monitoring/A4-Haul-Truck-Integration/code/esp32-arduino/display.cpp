// OLED Display: https://randomnerdtutorials.com/esp32-ssd1306-oled-display-arduino-ide/

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
  // For text size = 1
  // Max length characters = 21
  // Max rows = 8
  display->clearDisplay();

  display->setTextSize(1);      // Normal 1:1 pixel scale
  display->setTextColor(SSD1306_WHITE); // Draw white text
  display->setCursor(0, 0);     // Start at top-left corner

  display->println(msg);
  display->display();
}
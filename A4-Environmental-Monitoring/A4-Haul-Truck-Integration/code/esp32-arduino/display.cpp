// OLED Display: https://randomnerdtutorials.com/esp32-ssd1306-oled-display-arduino-ide/

#include "display.h"

namespace{
  Adafruit_SSD1306* display;
  uint8_t currentRow = 0;
  // uint8_t currentCol = 0;
  String lines[MAX_ROWS];
  bool screenOverflow = false;
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
  display->setTextSize(1);      // Normal 1:1 pixel scale
  display->setTextColor(SSD1306_WHITE); // Draw white text
  display->setCursor(0, 0);     // Start at top-left corner
}

void displayText(const String& msg)
{
  // For text size = 1
  // Max length characters = 21
  // Max rows = 8
  display->clearDisplay();

  // display->setTextSize(1);      // Normal 1:1 pixel scale
  // display->setTextColor(SSD1306_WHITE); // Draw white text
  screenOverflow = false;
  display->setCursor(0, 0);     // Start at top-left corner

  display->println(msg);
  display->display();
}


void displayOutput(String line)
{
  while(line.length() > 0)
  {
    // If line fits max limit, no processing required
    if(line.length() <= MAX_CHAR)
    {
      addLine(line);
      break;
    }

    uint8_t split = line.lastIndexOf(' ', MAX_CHAR);
    if(split == -1)
    {
      split = MAX_CHAR;
    }

    // Print line up to the last space before limit
    addLine(line.substring(0, split));

    // Process overflow string in the next loop
    line = line.substring(split);

    // Remove space at start of new line
    while(line.startsWith(" "))
    {
      line.remove(0, 1);
    }

  }
}

void addLine(const String& output)
{
  if(screenOverflow)
  {
    for(uint8_t i = 0; i < MAX_ROWS - 1; i++)
    {
      // Shift each line by 1 up
      lines[i] = lines[i + 1];
    }
    // Output to last line
    lines[MAX_ROWS - 1] = output;
  }
  else
  {
    lines[currentRow] = output;
  }

  display->clearDisplay();
  for(uint8_t i = 0; i < MAX_ROWS; i++)
  {
    // if(screenOverflow)
    // {
    //   display->setCursor(0, i * MAX_ROWS);
    // }
    display->println(lines[i]);
  }

  currentRow++;
  if(currentRow >= MAX_ROWS - 1)
  {
    screenOverflow = true;
  }
}
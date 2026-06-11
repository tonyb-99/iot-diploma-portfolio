// DS3231 RTC: https://randomnerdtutorials.com/esp32-ds3231-real-time-clock-arduino/

#include "clock.h"

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void initRTC()
{
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println("RTC initialised!");
  // Manually set timer
  // rtc.adjust(DateTime(2026, 5, 24, 11, 34, 0));
}

DateTime getTime()
{
  return rtc.now();
}

String getTimestamp()
{
  char syntax[] = "hh:mm:ss";
  return getTime().toString(syntax);
}

String getDate()
{
  char syntax[] = "DD-MM-YYYY";
  return getTime().toString(syntax);
}

String getDay(bool shortened)
{
  if(shortened)
  {
    char syntax[] = "DDD";
    return getTime().toString(syntax);
  }
  return daysOfTheWeek[getTime().dayOfTheWeek()];
}


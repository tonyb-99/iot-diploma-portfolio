#include <Arduino.h>

#define R_PIN 25
#define G_PIN 26
#define B_PIN 27
#define THERM_PIN 34

const float BETA = 3950;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  pinMode(THERM_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("OFF");
  // analogWrite(R_PIN, 0);
  // analogWrite(G_PIN, 0);
  // analogWrite(B_PIN, 0);
  // delay(1000);

  // Serial.println("RED");
  // analogWrite(R_PIN, 255);
  // analogWrite(G_PIN, 0);
  // analogWrite(B_PIN, 0);
  // delay(1000);

  // Serial.println("GREEN");
  // analogWrite(R_PIN, 0);
  // analogWrite(G_PIN, 255);
  // analogWrite(B_PIN, 0);
  // delay(1000);

  // Serial.println("BLUE");
  // analogWrite(R_PIN, 0);
  // analogWrite(G_PIN, 0);
  // analogWrite(B_PIN, 255);
  // delay(1000);

  // Serial.println("YELLOW");
  // analogWrite(R_PIN, 100);
  // analogWrite(G_PIN, 75);
  // analogWrite(B_PIN, 0);
  // delay(1000);
  int analogValue = analogRead(THERM_PIN);
  float celsius = 1 / (log(1 / (4095. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;  
  Serial.print("Temperature ('C): ");
  Serial.println(celsius);
  delay(3000);
}

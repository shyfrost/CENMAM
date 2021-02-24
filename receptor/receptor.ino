#include <LiquidCrystal.h>

#include <SoftwareSerial.h>

LiquidCrystal lcd1 (8, 9, 4, 5, 6, 7);

float temp = 0;
float hum = 0;
int alc = 0;

void setup() {
  Serial.begin(9600);
  lcd1.begin(16, 2);

}

void loop() {
  if(Serial.available());{
    temp = Serial.read();
    hum = Serial.read();
    alc = Serial.read();

  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("T: ");
  lcd1.print(temp);
  lcd1.print(" H: ");
  lcd1.print(hum);
  lcd1.setCursor(0, 1);
  lcd1.print("Alcohol: ");
  lcd1.print(alc);
  delay(1000);
  lcd1.clear();
  }
}

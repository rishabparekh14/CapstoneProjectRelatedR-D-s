#include <Arduino.h>
# define SensorPin 36

const int AirValue = 3550;   //you need to replace this value with Value_1
const int WaterValue = 1000;  //you need to replace this value with Value_2
//const int SensorPin = 36;
int soilMoistureValue = 0;
int soilmoisturepercent=0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  soilMoistureValue = analogRead(SensorPin);  //put Sensor insert into soil
Serial.print("SoilMositureValue:");
Serial.println(soilMoistureValue);
soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
Serial.println("SoilMoisturePercent:");
Serial.println(soilmoisturepercent);
delay(500);
}

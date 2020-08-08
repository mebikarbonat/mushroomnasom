//Author: Muhammad Azizi Bin Mohd Ariffin
//Email: mazizi@tmsk.uitm.edu.my

#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

#define DHTPIN 13     //Sensor 1
#define DHTPIN2 12   //Sensor 2

#define PINWater 15  //Water Pump
#define PINFan 2  //Fan

#define DHTTYPE DHT22   // DHT 22


DHT dht(DHTPIN, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("NARC Mushroom Control System \n"));
  Serial.println(F("Booting System.. \n"));
  Serial.println(F("NARC IOTBox 1.0 \n"));

  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Boot Sys!");
  lcd.setCursor(1,1);
  lcd.print("NARC IOTBox 1.0");
  delay(5000);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Sys OK");
  lcd.setCursor(1,1);
  lcd.print("NARC IOTBox 1.0");
  delay(5000);

  pinMode(PINWater, OUTPUT);
  pinMode(PINFan, OUTPUT);

  dht.begin();
  dht2.begin();

  digitalWrite(PINWater, HIGH); //Initially water pump and ventilation fan is closed
  digitalWrite(PINFan, HIGH);
}

void loop() {

  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();

  if (isnan(h) || isnan(t) ){
    Serial.println(F("Failed to read from DHT sensor 1!"));
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Failed to read");
    lcd.setCursor(0,1);
    lcd.print("from DHT sensor1!");
    return;
  }

  if (isnan(h2) || isnan(t2) ) {
    Serial.println(F("Failed to read from DHT sensor 2!"));
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Failed to read");
    lcd.setCursor(0,1);
    lcd.print("from DHT sensor2!");
    return;
  }

  Serial.print(F("Reading From Sensor 1 : "));
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("\n"));
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("S1 Temp: ");lcd.print(t,2);lcd.print("C'");
  lcd.setCursor(0,1);
  lcd.print("S1 Hum: ");lcd.print(h,2);lcd.print("%");
  delay(2000);

  Serial.print(F("Reading From Sensor 2 : "));
  Serial.print(F("Humidity: "));
  Serial.print(h2);
  Serial.print(F("%  Temperature: "));
  Serial.print(t2);
  Serial.print(F("\n"));
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("S2 Temp: ");lcd.print(t2,2);lcd.print("C'");
  lcd.setCursor(0,1);
  lcd.print("S2 Hum: ");lcd.print(h2,2);lcd.print("%");
  delay(2000);
  
  if ( t > 27.0 && t2 > 27.0) {
    digitalWrite(PINWater, LOW);
    digitalWrite(PINFan, LOW);
    Serial.println("Starting Water Pump and Ventilation Van to cool down temperature \n");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Start WaterPump");
    lcd.setCursor(0,1);
    lcd.print("&Fan");
    delay(10000); // Open for 10 Second
    digitalWrite(PINWater, HIGH);
    digitalWrite(PINFan, HIGH);
    Serial.println("Closing Water Pump and Ventilation Van\n");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Stop WaterPump");
    lcd.setCursor(0,1);
    lcd.print("&Fan");
    delay(10000);
  }

  if ( h < 60.0 && h2 < 60.0) {
    digitalWrite(PINWater, LOW);
    Serial.println("Starting Water Pump to increase humidity \n");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Start WaterPump");
    lcd.setCursor(0,1);
    lcd.print("");
    delay(10000); // Open for 10 Second
    digitalWrite(PINWater, HIGH);
    Serial.println("Closing Water Pump \n");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Stop WaterPump");
    lcd.setCursor(0,1);
    lcd.print("");
    delay(10000); // Open for 10 Second
  }

  if ( h > 80.0 && h2 > 80.0) {
    digitalWrite(PINFan, LOW);
    Serial.println("Starting Fan to reduce humidity \n");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Start Fan");
    lcd.setCursor(0,1);
    lcd.print("");
    delay(10000); // Open for 10 Second
    digitalWrite(PINFan, HIGH);
    Serial.println("Closing Fan \n");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Stop Fan");
    lcd.setCursor(0,1);
    lcd.print("");
    delay(10000); // Open for 10 Second
  }
}

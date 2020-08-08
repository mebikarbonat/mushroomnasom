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

int one = 0;
int two = 0;

int oneStat = 0;
int twoStat = 0;
int threeStat = 0;

float avgt = 0.0;
float avgh = 0.0;
int statSensor = 0;
int stop = 0;

DHT dht(DHTPIN, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

float h = 0.0;
float t = 0.0;
float h2 = 0.0;
float t2 = 0.0;
float tPrev = 0.0;
float hPrev = 0.0;

unsigned long currentMillis = 0;
unsigned long previous_sensor_Millis = 0;
unsigned long previous_one_Millis = 0;
unsigned long previous_two_Millis = 0;
unsigned long previous_three_Millis = 0;
unsigned long previous_stop_Millis = 0;
const unsigned long sensorInterval = 6000;
const unsigned long oneInterval = 20000;
const unsigned long twoInterval = 20000;
const unsigned long threeInterval = 20000;
const unsigned long stopInterval = 10000;

void setup() {
  Serial.begin(9600);
  Serial.println(F("NARC Mushroom Control System \n"));
  Serial.println(F("Booting System.. \n"));
  Serial.println(F("NARC IOTBox 1.0 \n"));

  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Booting Sys!");
  lcd.setCursor(1,1);
  lcd.print("NARC IOTBox 1.0");
  delay(2000);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("System OK");
  lcd.setCursor(1,1);
  lcd.print("NARC IOTBox 1.0");
  delay(2000);

  pinMode(PINWater, OUTPUT);
  pinMode(PINFan, OUTPUT);

  dht.begin();
  dht2.begin();

  digitalWrite(PINWater, HIGH); //Initially water pump and ventilation fan is closed
  digitalWrite(PINFan, HIGH);
}

void loop() {

  currentMillis = millis();
  if (currentMillis - previous_sensor_Millis >= sensorInterval){
  h = dht.readHumidity();
  t = dht.readTemperature();
  h2 = dht2.readHumidity();
  t2 = dht2.readTemperature();
  previous_sensor_Millis = currentMillis;
  }
   
  if (isnan(h) || isnan(t) || h==0 || t ==0 ){
    Serial.println(F("Failed to read from DHT sensor 1!"));
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Failed to read");
    lcd.setCursor(0,1);
    lcd.print("from DHT sensor1!");
    //return;
    statSensor = 1;
  }
  else {
    statSensor = 0;
  }

  if (isnan(h2) || isnan(t2) || h==0 || t ==0 ) {
    Serial.println(F("Failed to read from DHT sensor 2!"));
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Failed to read");
    lcd.setCursor(0,1);
    lcd.print("from DHT sensor2!");
    //return;
    statSensor = 1;
  }
  else {
    statSensor = 0;
  }

  avgt = (t + t2) / 2;
  avgh = (h + h2) / 2;
  
  if ( statSensor == 0 && t != tPrev && h != hPrev ) {
  Serial.print(F("Reading From Sensor 1 : "));
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("\n"));
  Serial.print(F("Reading From Sensor 2 : "));
  Serial.print(F("Humidity: "));
  Serial.print(h2);
  Serial.print(F("%  Temperature: "));
  Serial.print(t2);
  Serial.print(F("\n"));
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");lcd.print(avgt,2);lcd.print("C'");
  lcd.setCursor(0,1);
  lcd.print("Hum: ");lcd.print(avgh,2);lcd.print("%");
  tPrev = t;
  hPrev = h;
  }
  
  if ( t > 35.0 && t2 > 35.0 && statSensor == 0) {
    one = 1;
    if(digitalRead(PINWater)==HIGH && digitalRead(PINFan)==HIGH && stop==0){
      digitalWrite(PINWater, LOW);
      digitalWrite(PINFan, LOW);
      Serial.println("Starting Water Pump and Ventilation Van to cool down temperature \n");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("T:");lcd.print(avgt,2);lcd.print(" H:");lcd.print(avgh,2);
      lcd.setCursor(0,1);
      lcd.print("On WP&Fan");
    }
    if (currentMillis - previous_one_Millis >= oneInterval){
        digitalWrite(PINWater, HIGH);
        digitalWrite(PINFan, HIGH);
        previous_one_Millis = currentMillis;
        previous_stop_Millis = currentMillis-5000;
        stop = 1;
        Serial.println("Closing Water Pump and Ventilation Van\n");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("T:");lcd.print(avgt,2);lcd.print(" H:");lcd.print(avgh,2);
        lcd.setCursor(0,1);
        lcd.print("Off WP&Fan");
    }
  }
  else {
    one = 0;
  }

  if ( h < 60.0 && h2 < 60.0 && one == 0 && statSensor == 0) {
    two = 1;
      if(digitalRead(PINWater)==HIGH && stop==0){
      digitalWrite(PINWater, LOW);
      Serial.println("Starting Water Pump to increase humidity \n");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("T:");lcd.print(avgt,2);lcd.print(" H:");lcd.print(avgh,2);
      lcd.setCursor(0,1);
      lcd.print("On Waterpump Only");
    }
    if (currentMillis - previous_two_Millis >= twoInterval){
        digitalWrite(PINWater, HIGH);
        previous_two_Millis = currentMillis;
        previous_stop_Millis = currentMillis-5000;
        stop = 1;
        Serial.println("Closing Water Pump \n");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("T:");lcd.print(avgt,2);lcd.print(" H:");lcd.print(avgh,2);
        lcd.setCursor(0,1);
        lcd.print("Off Waterpump");
    }
  }
  else{
    two = 0;
  }

  if ( h > 80.0 && h2 > 80.0 && one == 0 && statSensor == 0 && two == 0) {
      if(digitalRead(PINFan)==HIGH && stop==0){
      digitalWrite(PINFan, LOW);
      Serial.println("Starting Fan to reduce humidity \n");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("T:");lcd.print(avgt,2);lcd.print(" H:");lcd.print(avgh,2);;
      lcd.setCursor(0,1);
      lcd.print("On Fan Only");
    }
    if (currentMillis - previous_three_Millis >= threeInterval){
        digitalWrite(PINFan, HIGH);
        previous_three_Millis = currentMillis;
        previous_stop_Millis = currentMillis-5000;
        stop = 1;
        Serial.println("Closing Fan \n");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("T:");lcd.print(avgt,2);lcd.print(" H:");lcd.print(avgh,2);
        lcd.setCursor(0,1);
        lcd.print("Off Fan");
    }
  }
  avgt = 0.0;
  avgh = 0.0;
  if(stop==1){
    if (currentMillis - previous_stop_Millis >= stopInterval){
      stop=0;
      previous_stop_Millis = currentMillis;
    }
  }
  //delay(2000);
}

/*
##########################
# BETTER UPDATED VERSION #
##########################
*/

//__Pins__


//UltraSonicSensor
const int P_TRIG = 13;
const int P_ECHO = A2;
//InfraRed
const int P_RECV = 8;
//Measure
const int P_TEMP = A1;
const int P_MOIST = A0;
const int P_LIGHT = A3;
//Buzzer
const int P_BUZZER = 12;
//LCD
const int RS = 11;
const int RW = 10;
const int E = 9;
const int D7 = 4;
const int D6 = 5;
const int D5 = 6;
const int D4 = 7;

//__Variables__


//UltraSonicSensor
const int maximumrange = 3000;
const int minimumrange = 0;
long duration, distance;
//Misc
int mode;
bool firstround = 1;
const int processing = 0;


//__StartUp__

#include <LiquidCrystal.h>
#include <DS3231.h>
#include <IRremote.h>
#include <math.h>

IRrecv irrecv(P_RECV);
decode_results results;
DS3231 rtc(SDA, SCL);
LiquidCrystal lcd(RS, RW, E, D4, D5, D6, D7);

void setup() {
  Serial.begin(9600);

  pinMode(P_BUZZER, OUTPUT);
  pinMode(P_TRIG, OUTPUT);

  rtc.begin();
  lcd.begin(16, 2);
  irrecv.enableIRIn();
  if (processing == 0) {
    Serial.print("[*] Setup completed successfully");
  }
}

void loop() {
  mode = 3;
  if (mode == 0) {
    FuncsetTime();
  } else if (mode == 1) {
    FuncsetTemp();
  } else if (mode == 2){
    FuncsetMoist();
  }
  delay(50);
}
void FuncsetTime() {
  char* dayOfWeek = rtc.getDOWStr();
  char* Mytime = rtc.getTimeStr();
  char* date = rtc.getDateStr();

  lcd.setCursor(0, 0);
  lcd.print("Zeit:   ");
  lcd.print(Mytime);
  lcd.setCursor(0, 1);
  lcd.print("Datum:  ");
  lcd.print(date);
  delay(20);
}


void FuncsetTemp() {
  float RawADC = analogRead(A1);
  double Temp;
  Temp = log(10000.0 * ((1024.0 / RawADC - 1)));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
  Temp = Temp - 273.15;
  Temp = Temp / 5.42;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Temperatur");
  float out = Temp;
  lcd.setCursor(4, 1);
  lcd.print(out);
  lcd.setCursor(4, 4);
  lcd.print(" °C");
  Serial.print(out);
  if (processing == 0) {
    Serial.println(" °C");
  }
}

void FuncsetMoist() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Luftfeuchtigkeit");

  int temp = analogRead(P_MOIST);
  int out = temp;
  lcd.setCursor(0, 1);
  lcd.print(out);
  lcd.print("%");
  Serial.print(out);
  Serial.println("%");
}

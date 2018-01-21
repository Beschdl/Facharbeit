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
const int P_LIGHT = A7;
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

const int P_PROC = 3;
//__Variables__

//Melody
char note1[] = "ccggaagffeeddc ";
int beat1[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo1 = 200;

//UltraSonicSensor
const int maximumRange = 1400;
const int minimumRange = 0;
long duration, distance;
//Misc
int mode = 4;
int processing = 0;

//__StartUp__

#include <LiquidCrystal.h>
#include <DS3231.h>
#include <math.h>

DS3231 rtc(SDA, SCL);
LiquidCrystal lcd(RS, RW, E, D4, D5, D6, D7);

void setup() {
  Serial.begin(9600);

  pinMode(P_BUZZER, OUTPUT);
  pinMode(P_TRIG, OUTPUT);

  rtc.begin();
  lcd.begin(16, 2);
  if (processing == 0) {
    Serial.print("[*] Setup completed successfully");
  }
}

void loop() {
  checkMode();
  checkProcessing();
  if (mode == 1) {
    FuncsetTime();
  } else if (mode == 2) {
    FuncsetTemp();
  } else if (mode == 3) {
    FuncsetMoist();
  } else if (mode == 4) {
    FuncsetBrightness();
  } else if (mode == 5) {
    FuncsetLength();
  } else if (mode == 6) {
    FuncsetMelody();
  }
  delay(10);
}

void checkProcessing() {
  int act = digitalRead(P_PROC);
  if (act == HIGH) {
    processing = 1;
  } else if (act == LOW) {
    processing = 0;
  }
}

void checkMode() {
  int value = analogRead(A4);
  mode = map(value, 0, 1023, 1, 6);
  Serial.println(mode);
}


void FuncsetTime() {
  char* dayOfWeek = rtc.getDOWStr();
  char* Mytime = rtc.getTimeStr();
  char* Mydate = rtc.getDateStr();

  lcd.setCursor(0, 0);
  lcd.print("Zeit:   ");
  lcd.print(Mytime);
  lcd.setCursor(0, 1);
  lcd.print("Datum:  ");
  lcd.print(Mydate);
  lcd.setCursor(13, 1);
  lcd.print(".18");
}


void FuncsetTemp() {
  float RawADC = analogRead(A1);
  double Temp;
  Temp = log(10000.0 * ((1024.0 / RawADC - 1)));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
  Temp = Temp - 273.15;
  Temp = Temp / 5.42;

  lcd.setCursor(0, 0);
  lcd.print("   Temperatur    ");
  int out = Temp;
  lcd.setCursor(0, 1);
  lcd.print("     ");
  lcd.print(out);
  lcd.setCursor(7, 4);
  lcd.print(" \xdf");
  lcd.print("C         ");
  if (processing == 0) {
    Serial.print(out);
    Serial.println(" °C");
  } else if (processing == 1) {
    float processing_out = map(out, 0, 100, 0, 1023);
    Serial.println(processing_out);
  }
}

void FuncsetMoist() {
  lcd.setCursor(0, 0);
  lcd.print("  Feuchtigkeit  ");
  int temp = analogRead(P_MOIST);
  int out = temp;
  lcd.setCursor(4, 1);
  lcd.print("   ");
  int percent = map(out, 0, 1023, 0, 100);
  lcd.print(percent);
  lcd.print("%             ");
  if (processing == 0) {
    Serial.print(percent);
    Serial.println("%");
  } else {
    Serial.println(out * 2);
  }
}
void FuncsetBrightness() {
  lcd.setCursor(2, 0);
  lcd.print("Helligkeit  ");

  int temp = analogRead(P_LIGHT);
  int out = temp;
  lcd.setCursor(6, 1);
  lcd.print(out);
  lcd.print("          ");
  Serial.println(out);
  if (processing == 0) {
    Serial.println("%    ");
  }
}
void FuncsetLength() {
  lcd.setCursor(0, 0);
  lcd.print("      L\xe1nge   ");
  digitalWrite(P_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(P_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(P_TRIG, LOW);
  duration = pulseIn(P_ECHO, HIGH);
  distance = duration / 58.2;
  lcd.setCursor(6, 1);
  lcd.print(distance);
  lcd.print(" cm      ");
  if (distance >= maximumRange || distance <= minimumRange) {
    Serial.println("-1");
  }
  else {
    if (processing == 0) {
      Serial.print(distance);
      Serial.println(" cm   s");
    } else if (processing == 1) {
      float processing_out = map(distance, 0, 150, 0, 1023);
      Serial.println(processing_out);
    }
  }
}


void FuncsetMelody() {
  int length = 15;
  for (int i = 0; i < length; i++) {
    if (note1[i] == ' ') {
      delay(beat1[i] * tempo1); // rest
    } else {
      playNote(note1[i], beat1[i] * tempo1);
      delay(tempo1 / 2);
    }
  }
}


void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(P_BUZZER, HIGH);
    delayMicroseconds(tone);
    digitalWrite(P_BUZZER, LOW);
    delayMicroseconds(tone);
  }
}
void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 't' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956, 988 };
  for (int i = 0; i < 9; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

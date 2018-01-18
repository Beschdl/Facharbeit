/*
  #################################
  #      MESSGERÄT                #
  #   Messbare Einheiten:         #
  #    -Temperatur                #
  #   -Lichtverhältnisse          #
  #   -Luftfeutchtigkeit          #
  #   -Geschwindigkeit            #
  #   -Geschräusche bzw.          #
  #    Lautstärke                 #
  #    -Zeit                      #
  #    -Länge                     #
  #    +integrierte Uhr           #
  #################################
*/
// ####### NOTES

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
char note1[] = "ccggaagffeeddc ";
int beat1[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo1 = 200;
char note2[] = "eegedct";
int beat2[] = {2, 1, 1, 1, 1, 4, 4};
int tempo2 = 150;
char note3[] = "";
int beat3[] = {};
int tempo3 = 500;
//  ######## BASIC LIBRARIES

#include <LiquidCrystal.h>
#include <IRremote.h>
#include <DS3231.h>     // Change library name, if doesn't exist


//  ############# IMPORTANT CONSTANTS

const int RECV_PIN = 13;  //Pin for IR reciever
const int TRIG_PIN = 12;  //Pin for UltraSonicSensor (Send Waves)
const int ECHO_PIN = A2;  //Pin for UltraSonicSensor (Recieve Waves);
const int DELAY_USS = 10;
const int TEMP_PIN = A1;  //For temperature measuring
const int MOIST_PIN = A0;
const int BUZZER = 8;
const int RS = 11;
const int RW = 9;
const int ENABLE = 10;
const int D4 = 5;
const int D5 = 4;
const int D6 = 3;
const int D7 = 2;


//  ########

int mode = 0;
int firstround = 1;
const int processing = 0;

//  ####### BASIC FUNCTIONS

IRrecv irrecv(RECV_PIN);
decode_results results;
DS3231 rtc(SDA, SCL);
LiquidCrystal lcd(RS, RW, ENABLE, D4, D5, D6, D7);




void setup() {
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  // while(!Serial) {} //only for Arduino Leonardo

  pinMode(TRIG_PIN, OUTPUT);

  rtc.begin();
  lcd.begin(16, 2);
  irrecv.enableIRIn();
  Serial.println("{*] Setup successfully finished");
  setMelody(2);
};


void loop() {
  switch (readIR()) {
    case 0:
      MysetTime();
    case 1:
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Temperatur");
      setTemp(analogRead(TEMP_PIN));
    case 2:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Helligkeit");
      MysetBrightness();
    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Feuchtigkeit");
      setHumidity();
    case 4:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Geschwindigkeit");
      MysetSpeed();
    case 5:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Lautstärke");
      MysetVolume();
    case 6:
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Zetit");
      setStopper();
    case 7:
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("L\xe1nge");
      setLength();
    default:
      lcd.setCursor(0, 0);
      lcd.print(" ERROR: Eingabe");
      lcd.setCursor(0, 1);
      lcd.print(" nicht vergeben");
  }
  delay(10);
  if (firstround == 1) {
    Serial.println("[*] First round finished");
    firstround = 0;
  }
}

int readIR() {
  long irValue = results.value;
  if (irrecv.decode(&results)) {
    switch (irValue) {
      case 0xFF6897:  mode = 0;
      case 0xFF30CF:  mode = 1;
      case 0xFF18E7:  mode = 2;
      case 0xFF7A85:  mode = 3;
      case 0xFF10EF:  mode = 4;
      case 0xFF38C7:  mode = 5;
      case 0xFF5AA5:  mode = 6;
      case 0xFF42BD:  mode = 7;
      case 0xFF4AB5:  mode = 8;
      case 0xFF52AD:  mode = 9;
    }
    irrecv.resume();
  }
  return mode;
  Serial.println(mode);
}


void setStopper() {
  int time = 0;
  int stop;
  setStopperIntro();
  if (readIR() == 0) {
    while (stop != 1) {
      lcd.setCursor(3, 1);
      lcd.print(time);
      delayMicroseconds(1);
      time += 1;
      if (readIR() == 9) {
        stop = 1;
        lcd.setCursor(0, 1);
        lcd.print("Final time: ");
        lcd.print(time);
      }
    }
  }
}

void setStopperIntro() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Zum Starten");
  lcd.setCursor(0, 1);
  lcd.print("Dr\xf5");
  lcd.print("cke \"0\"");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Zum Stoppen");
  lcd.setCursor(0 , 1);
  lcd.print("Dr\xf5");
  lcd.print("cke \"9\"");
  delay(2000);
  lcd.clear();
}






void setLength() {

  //Send Signal
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(DELAY_USS);
  digitalWrite(TRIG_PIN, LOW);
  // Recieved Signal

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;

  if (distance == 0 || distance <= 400) {
    lcd.setCursor(0, 1);
    lcd.print("Error/Zu weit");
    if (distance <= 400 && distance <= 1000) {
      lcd.setCursor(0, 1);
      lcd.print("Zu weit");
    }
  } else {

    lcd.setCursor(6, 1);
    lcd.print(distance);
    lcd.print("cm");

  }
  delay(1);

  if (processing == 1) {
    Serial.println(distance);
  }
}




void setTemp(int TempIn) {
  int Factor;
  int Temp;
  Temp = TempIn * Factor;
  lcd.setCursor(5, 0);
  lcd.print(Temp);
  lcd.print("°C");

}

void MysetTime() {

  lcd.clear();
  char* dayOfWeek = rtc.getDOWStr();
  char* Mytime = rtc.getTimeStr();
  char* date = rtc.getDateStr();

  lcd.setCursor(0, 0);
  lcd.print("Zeit:  ");
  lcd.print(Mytime);
  lcd.setCursor(1, 0);
  lcd.print("Datum:  ");
  lcd.print(date);
}


void MysetBrightness() {

}

void MysetSpeed() {

}

void setHumidity() {

}

void MysetVolume() {

}
void setMelody(int song) {
  if (song == 1) {
    int length = 15;
    for (int i = 0; i < length; i++) {
      if (note1[i] == ' ') {
        delay(beat1[i] * tempo1); // rest
      } else {
        playNote(note1[i], beat1[i] * tempo1);
        delay(tempo1 / 2);
      }
    }
  } else if (song == 2) {
    int length = 7;
    for (int i = 0; i < length; i++) {
      if (note2[i] == ' ') {
        delay(beat2[i] * tempo2); // rest
      } else {
        playNote(note2[i], beat2[i] * tempo2);
        delay(tempo2 / 2);
      }
    }
  }
}
void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(BUZZER, HIGH);
    delayMicroseconds(tone);
    digitalWrite(BUZZER, LOW);
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

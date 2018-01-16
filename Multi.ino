/*
  #################################
  #    MESSGERÄT #
  # Messbare Einheiten:   #
  #   -Temperatur #
  # -Lichtverhältnisse  #
  # -Luftfeutchtigkeit      #
  # -Geschwindigkeit        #
  # -Geschräusche bzw.      #
  #   Lautstärke    #
  #   -Zeit   #
  #   -Länge    #
  #  +integrierte Uhr #
  #################################
*/
//  ######## BASIC LIBRARIES



#include <LiquidCrystal.h>
#include <IRremote.h>
#include <DS3231.h>     // Change library name, if doesn't exist

int mode;

//  ############# IMPORTANT CONSTANTS

const int RECV_PIN = 13;  //Pin for IR reciever
const int TRIG_PIN = 12;  //Pin for UltraSonicSensor (Send Waves)
const int ECHO_PIN = A0;  //Pin for UltraSonicSensor (Recieve Waves);
const int DELAY_USS = 10;
const int TEMP_PIN = A1;  //For temperature measuring
const int RS = 11;
const int RW = 10;
const int ENABLE = 9;
const int D4 = 8;
const int D5 = 7;
const int D6 = 6;
const int D7 = 5;

//  ######## E.O. IMPORTANT CONSTANTS



//  ####### BASIC FUNCTIONS

IRrecv irrecv(RECV_PIN);
decode_results results;
DS3231 rtc(SDA, SCL);
LiquidCrystal lcd(RS, RW, ENABLE, D4, D5, D6, D7);




void setup() {
  Serial.begin(9600);

  // while(!Serial) {} //only for Arduino Leonardo

  pinMode(TRIG_PIN, OUTPUT);

  rtc.begin();
  lcd.begin(16, 2);
  irrecv.enableIRIn();
};


void loop() {
  lcd.clear();
  switch (readIR()) {
    case 1:
      MysetTime();
    case 2:
      lcd.setCursor(3, 0);
      lcd.print("Temperatur");
      setTemp(analogRead(TEMP_PIN));
    case 3:
      lcd.setCursor(1, 0);
      lcd.print("Helligkeit");
      MysetBrightness();
    case 4:
      lcd.setCursor(0, 0);
      lcd.print("Feuchtigkeit");
      setHumidity();
    case 5:
      lcd.setCursor(0, 0);
      lcd.print("Geschwindigkeit");
      MysetSpeed();
    case 6:
      lcd.setCursor(0, 0);
      lcd.print("Lautstärke");
      MysetVolume();
    case 7:
      lcd.setCursor(6, 0);
      lcd.print("Zeit");
      setStopper();
    case 8:
      lcd.setCursor(4, 0);
      lcd.print("Länge");
      setLength();
    default:
      lcd.setCursor(0, 0);
      lcd.print(" ERROR: Eingabe");
      lcd.setCursor(0, 1);
      lcd.print(" nicht vergeben");
  }
  delay(10);
}

int readIR() {
  int irValue = results.value;
  switch (irValue) {
    case 0xFF6897:  mode = 0;
      break;
    case 0xFF30CF:  mode = 1;
      break;
    case 0xFF18E7:  mode = 2;
      break;
    case 0xFF7A85:  mode = 3;
      break;
    case 0xFF10EF:  mode = 4;
      break;
    case 0xFF38C7:  mode = 5;
      break;
    case 0xFF5AA5:  mode = 6;
      break;
    case 0xFF42BD:  mode = 7;
      break;
    case 0xFF4AB5:  mode = 8;
      break;
    case 0xFF52AD:  mode = 9;
      break;
  }
  return mode;
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
  lcd.setCursor(0, 0);
  lcd.print("Zum Starten");
  lcd.setCursor(0, 1);
  lcd.print("Druecke \"0\"");
  delay(2000);
  lcd.setCursor(0, 0);
  lcd.print("Zum Stoppen");
  lcd.setCursor(0 , 1);
  lcd.print("Druecke \"9\"");
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
}




void setTemp(int TempIn) {
  int Factor;
  int Temp;
  Temp = TempIn * Factor;
  lcd.setCursor(5, 0);
  lcd.print(Temp);
  lcd.print("°C");

}

void setTime() {

  lcd.clear();
  int dayOfWeek = rtc.getDOWStr();
  int time = rtc.getTimeStr();
  int date = rtc.getDateStr();

  lcd.setCursor(0, 0);
  lcd.print("Zeit:  ");
  lcd.print(time);
  lcd.setCursor(1, 0);
  lcd.print("Datum:  ");
  lcd.print(date);
}



void MysetTime() {


}

void MysetBrightness() {

}

void MysetSpeed() {

}

void setHumidity() {

}

void MysetVolume() {

}

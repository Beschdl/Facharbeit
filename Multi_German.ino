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

//__Pins__
/*
   Das hier sind die Pins, also die Anschlüsse and dem Arduino.
   Jeder der Pins fängt mit "P_" and, um es als Pin zu kennzeichnen.
   Außerdem ist jeder Buchstabe groß geschrieben, da das bei Konstanten üblich ist.
*/

// Ultraschallsensor
const int P_AUSLOESER = 13;
const int P_ECHO = A2;

// Measure
const int P_TEMPERATUR = A1;
const int P_FEUCHTIGKEIT = A0;
const int P_LICHT = A7;

// Buzzer
const int P_BUZZER = 12;

// LCD
const int RS = 11;
const int RW = 10;
const int E = 9;
const int D7 = 4;
const int D6 = 5;
const int D5 = 6;
const int D4 = 7;

const int P_PROCESSING = 3;


//__Variables__


// Melodie
char Note1[] = "ccggaagffeeddc ";
int Rhythmus1[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int Tempo1 = 200;

// Ultraschallsensor
const int maximalerAbstand = 1400;
const int minimalerAbstand = 0;
long Dauer, Distanz;

// Sonstige
int Modus, processing;

//__StartUp__
/*
   Alles hier ist um die "Libraries" einzubinden und zu konfigurieren.
   Das sind externe Codes, die man aufgrund ihrer Größe nicht gerne in den Code direkt einfügt.
   Außerdem sind sie so flexibler und übersichtlicher.
*/

#include <LiquidCrystal.h>
#include <DS3231.h>

DS3231 rtc(SDA, SCL);
LiquidCrystal lcd(RS, RW, E, D4, D5, D6, D7);

	// Hier kommt das Setup

void setup() {
  /*
     Dieser Code hier wird einmal ausgeführt, um einige Einstellungen zu konfigureren.
     Hier werden z.B. Die RTC (Die Uhr), oder das LCD (Das Display) gestartet.
  */
  Serial.begin(9600);

  pinMode(P_BUZZER, OUTPUT);
  pinMode(P_AUSLOESER, OUTPUT);

  rtc.begin();
  lcd.begin(16, 2);
  if (processing == 0) {
    Serial.println("[*] Setup erfolgreich abgeschlossen!");
  } else if (processing == 1) {
    Serial.println(1023);
    delay(1000);
  }
}


void loop() {
  /*
     Dieser Teil wird immer wiederholt.
     In diesem Falle wird die Funktion alle 10 Millisekunden ausgeführt.
     In dieser Zeti wird überprüft, welche Funktion ausgewählt wurde, und führt anschließend die Funktion aus.
     Wenn z.B. Modus 1 ausgewählt wurde, zeigt der Arduino  die Zeit und das Datum an.
  */

  ueberpruefeModus();
  ueberpruefeProcessing();
  if (Modus == 1) {
    Funktion_Time();
  } else if (Modus == 2) {
    Funktion_Temperatur();
  } else if (Modus == 3) {
    Funktion_Feuchtigkeit();
  } else if (Modus == 4) {
    Funktion_Helligkeit();
  } else if (Modus == 5) {
    Funktion_Laenge();
  } else if (Modus == 6) {
    Funktion_Melodie();
  } else {
    if (processing == 0) {
      Serial.println("[!] Etwas ist falsch gelaufen, ungültiger Modus");
      delay(100);
    } else if (processing == 1) {
      Serial.println("0");
      delay(100);
    }
  }
  delay(10);
}


void ueberpruefeProcessing() {

  // Diese Funktion Überprüft einen Schalter, der umgeschaltet werden kann auf entweder Textausgabe, oder eine reine Zahlenausgabe für das Programm "Processing".

  int Processing_Status = digitalRead(P_PROCESSING);

  if (Processing_Status == HIGH) {
    processing = 1;
  } else if (Processing_Status == LOW) {
    processing = 0;
  }
}

void ueberpruefeModus() {
  int value = analogRead(A4);
  Modus = map(value, 0, 1023, 1, 6);
}


void Funktion_Time() {

  // Der Teil übernimmt die Zeit von der angeschlossenen RTC. Dafür wird eine Library benötigt, da ansonsten der Code ziemlich lang sein würde.

  char* Zeit = rtc.getTimeStr();
  char* Datum = rtc.getDateStr();

  lcd.setCursor(0, 0);
  lcd.print("Zeit:   ");
  lcd.print(Zeit);
  lcd.setCursor(0, 1);
  lcd.print("Datum:  ");
  lcd.print(Datum);
  lcd.setCursor(13, 1);
  lcd.print(".18");
}


void Funktion_Temperatur() {

  // Diese Funktion misst die Temperatur über den Thermistor.

  lcd.setCursor(0, 0);
  lcd.print("   Temperatur    ");

  int Vo;
  float R1 = 10000;
  float logR2, R2, T, Tc;
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

  Vo = analogRead(P_TEMPERATUR);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  Tc = T - 273.15;
  int ausgabe = Tc;

  lcd.setCursor(0, 1);
  lcd.print("     ");
  lcd.print(ausgabe);
  lcd.setCursor(7, 4);
  lcd.print(" \xdf");
  lcd.print("C         ");

  if (processing == 0) {
    Serial.print(ausgabe);
    Serial.println(" °C");
  } else if (processing == 1) {
    float processing_ausgabe = map(ausgabe, 0, 100, 0, 1023);
    Serial.println(processing_ausgabe);
  }
}


void Funktion_Feuchtigkeit() {

  //Durch diese Funktion wird die Feuchtigkeit gemessen.

  lcd.setCursor(0, 0);
  lcd.print("  Feuchtigkeit  ");

  int temporaer = analogRead(P_FEUCHTIGKEIT);
  int ausgabe = temporaer;

  lcd.setCursor(4, 1);
  lcd.print("   ");
  int Prozent = map(ausgabe, 0, 1023, 0, 100);
  lcd.print(Prozent);
  lcd.print("%             ");

  if (processing == 0) {
    Serial.print(Prozent);
    Serial.println("%");
  } else {
    Serial.println(ausgabe * 2);
  }
}



void Funktion_Helligkeit() {

  //Bei dieser Funktion wird die Helligkeit gemessen

  lcd.setCursor(2, 0);
  lcd.print("Helligkeit  ");

  int temporaer = analogRead(P_LICHT);
  int ausgabe = temporaer;

  lcd.setCursor(6, 1);
  lcd.print(ausgabe);
  lcd.print("          ");
  Serial.print(ausgabe);

  if (processing == 0) {
    Serial.println("  (auf einer Skala von 0 bis 1023)");
  } else if (processing == 1) {
    Serial.println();
  }
}



void Funktion_Laenge() {

  lcd.setCursor(0, 0);
  lcd.print("      L\xe1nge   ");

  digitalWrite(P_AUSLOESER, LOW);
  delayMicroseconds(2);
  digitalWrite(P_AUSLOESER, HIGH);
  delayMicroseconds(10);
  digitalWrite(P_AUSLOESER, LOW);
  Dauer = pulseIn(P_ECHO, HIGH);

  Distanz = Dauer / 29.1;
  Distanz = Distanz / 2;

  lcd.setCursor(6, 1);
  lcd.print(Distanz);
  lcd.print(" cm      ");

  if (Distanz >= maximalerAbstand || Distanz <= minimalerAbstand) {
    Serial.println("-1");
  }
  else {
    if (processing == 0) {
      Serial.print(Distanz);
      Serial.println(" cm   s");
    } else if (processing == 1) {
      float processing_ausgabe = map(Distanz, 0, 150, 0, 1023);
      Serial.println(processing_ausgabe);
    }
  }
}


		// Alles ab hier bis ganz unten ist für die Melodie zuständig. Auch wenn die Melodie eigentlich die kleinste Funktion in meinem Projekt ist, ist der Code dafür am längsten.


void Funktion_Melodie() {

  int length = 15;

  for (int i = 0; i < length; i++) {
    if (Note1[i] == ' ') {
      delay(Rhythmus1[i] * Tempo1); // rest
    } else {
      spieleNote(Note1[i], Rhythmus1[i] * Tempo1);
      delay(Tempo1 / 2);
    }
  }
}


void spieleTon(int Ton, int Dauer) {

  for (long i = 0; i < Dauer * 1000L; i += Ton * 2) {
    digitalWrite(P_BUZZER, HIGH);
    delayMicroseconds(Ton);
    digitalWrite(P_BUZZER, LOW);
    delayMicroseconds(Ton);
  }
}

void spieleNote(char Note, int Dauer) {

  char names[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b'};
  int Toene[] = {1915, 1700, 1519, 1432, 1275, 1136, 1014};

  for (int i = 0; i < 7; i++) {
    if (names[i] == Note) {
      spieleTon(Toene[i], Dauer);
    }
  }

}

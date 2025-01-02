// KNK v2 - blink feature
//

#include <OneWire.h>
#include <DallasTemperature.h>

// Zmienne do obslugi migania diod
  unsigned long currMillis = 0;             // zmienna do migania diodami
  unsigned long prevMillisG = 0;            // zmienna do migania diody Green
  unsigned long prevMillisY = 0;            // zmienna do migania diody Yellow
  unsigned long prevMillisR = 0;            // zmienna do migania diody Red
  const unsigned int interval = 1500;       // miganie normalne
  const unsigned int interval1 = 500;       // miganie szybkie
  const unsigned int interval2 = 100;       // miganie ostrzegawcze
  bool tempLedGstate = LOW;                 // stan diody Green
  bool tempLedYstate = LOW;                 // stan diody Yellow
  bool tempLedRstate = LOW;                 // stan diody Red

// Zmienne temperatury
  // float temperatura = 0;   // temperatura z termometru
  int temp = 0;               // temeratura po zaokragleniu do pelnych stopni

// Zmienne poziomu wody
  bool poziomL = LOW;
  bool poziomH = HIGH;

// Ustalenie temperatur granicznych (histereza)
  uint8_t tempH = 10;   // Temperatura wysoka (grzałka wylaczona) - 10
  uint8_t tempL = 3;    // Temperatura niska (grzalka wlaczona) - 3

// Piny wyjściowe diod termometru
  const uint8_t termLedG = 1;     // Zielona - temperatura wysoka (pin3)
  const uint8_t termLedY = 2;     // Zolta - temperatura dopuszczalna (pin4)
  const uint8_t termLedR = 3;     // Czerwona - temperatura krytyczna (pin5)
  
// Pin wyjściowy przekaźnika grzałki
  const uint8_t grzalkaRel = 4;   // Moduł przekaznika 5V  (pin6)
  uint8_t grzalkaStan = 0;        // grzałka wyłączona

// Piny wejściowe dla czujnikow poziomu wody
  const uint8_t poziomLpin = 9;   // Czujnik dolny (pin15)
  const uint8_t poziomHpin = 10;  // Czujnik górny (pin16)

// Piny wyjściowe diod poziomu wody
  const uint8_t poziomLedH = 5;   // Czerwona - poziom wysoki (pin11)
  const uint8_t poziomLedN = 6;   // Zielona - poziom odpowiedni (pin12)
  const uint8_t poziomLedL = 7;   // Czerwona - poziom niski (pin13)

String waterlevel = "";
String var = "";

// Pin wejsciowy termometrów
#define ONE_WIRE_BUS 8 // Wejście magistrali OneWire (pin14)

// Podłączenie termometrów DS18D20 to magistrali oneWire
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress pipeThermometer;

// int sensorPin = A0;   // potencjometr do testu temperatury

// USTAWIENIA - Początek
void setup(void) {
  // Inicjalizacja termometru
    sensors.begin();

  // Ustawienie rozdzielczości termometru
    sensors.setResolution(pipeThermometer, 12);
  
  // Ustawienia dla diod termometru
    pinMode(termLedG, OUTPUT);  // Zielona
    pinMode(termLedY, OUTPUT);  // Zolta
    pinMode(termLedR, OUTPUT);  // Czerwona
  
  // Ustawienia dla przekaznika grzalki
    pinMode(grzalkaRel, OUTPUT); // Przekaznik 5V i dioda niebieska

  // Ustawienia dla diod poziomu wody
    pinMode(poziomLedH, OUTPUT); // Czerwona - poziom wysoki
    pinMode(poziomLedN, OUTPUT); // Zielona - poziom odpowiedni
    pinMode(poziomLedL, OUTPUT); // Czerwona - poziom niski

  // Ustawienia dla czujnikow poziomu
    pinMode(poziomLpin, INPUT); // Poziom dolny
    pinMode(poziomHpin, INPUT); // Poziom gorny

  // Test diod po restarcie układu
    digitalWrite(poziomLedH, HIGH);
    digitalWrite(poziomLedN, HIGH);
    digitalWrite(poziomLedL, HIGH);
    digitalWrite(termLedR, HIGH);
    digitalWrite(termLedY, HIGH);
    digitalWrite(termLedG, HIGH);
    digitalWrite(grzalkaRel, HIGH);
    delay (3000);
    digitalWrite(poziomLedH, LOW);
    digitalWrite(poziomLedN, LOW);
    digitalWrite(poziomLedL, LOW);
    digitalWrite(termLedR, LOW);
    digitalWrite(termLedY, LOW);
    digitalWrite(termLedG, LOW);

  // Wyłączenie grzałki (stan początkowy)
    digitalWrite(grzalkaRel, LOW);
    grzalkaStan = 0; // grzałka wyłączona
}
// USTAWIENIA - Koniec

void loop(void) {

  // temperatura z potencjometru do testow
  // temp = round(map(analogRead(sensorPin), 0, 1023, -5, 15));

  currMillis = millis();

  // Odczyt temperatury
  sensors.requestTemperatures(); // Pobieranie temperatury
  // temperatura = sensors.getTempCByIndex(0); // Przypisanie temperatury odczytanej z termometru do zmiennej
  temp = round(sensors.getTempCByIndex(0)); // Przypisanie temperatury odczytanej z termometru do zmiennej i zaokraglenie do pelnych stopni
  
  // Odczyt stanu czujników poziomu wody
  poziomL = digitalRead(poziomLpin);
  poziomH = digitalRead(poziomHpin);

  // Funkcja wskazywania poziomu wody na diodach LED
  poziomWody();
  
  // Załączanie grzałki na podstawie histerezy
  grzalka();

  // Obsluga diod temperatury
  temperaturaLed();
}


void temperaturaLed() {
  if (temp >= 15) {
    blinkLedG(interval2, currMillis);
    digitalWrite(termLedY, LOW);
    digitalWrite(termLedR, LOW);
  } 
  else if (temp > 11) {
    blinkLedG(interval1, currMillis);
    digitalWrite(termLedY, LOW);
    digitalWrite(termLedR, LOW);
  } 
  else if (temp <= 11 && temp >= 1) {
    switch (temp) {
    case 11:
    blinkLedG(interval, currMillis);
    digitalWrite(termLedY, LOW);
    digitalWrite(termLedR, LOW);
    break;
    case 10:
    digitalWrite(termLedG, HIGH);
    digitalWrite(termLedY, LOW);
    digitalWrite(termLedR, LOW);
    break;
    case 9:
    digitalWrite(termLedG, HIGH);
    digitalWrite(termLedY, HIGH);
    digitalWrite(termLedR, LOW);
    break;
    case 8:
    blinkLedG(interval, currMillis);
    digitalWrite(termLedY, HIGH);
    digitalWrite(termLedR, LOW);
    break;
    case 7:
    blinkLedG(interval, currMillis);
    blinkLedY(interval, currMillis);
    digitalWrite(termLedR, LOW);
    break;
    case 6:
    digitalWrite(termLedG, LOW);
    blinkLedY(interval, currMillis);
    digitalWrite(termLedR, LOW);
    break;
    case 5:
    digitalWrite(termLedG, LOW);
    blinkLedY(interval, currMillis);
    digitalWrite(termLedR, HIGH);
    break;
    case 4:
    digitalWrite(termLedG, LOW);
    digitalWrite(termLedY, LOW);
    digitalWrite(termLedR, HIGH);
    break;
    case 3:
    digitalWrite(termLedG, LOW);
    digitalWrite(termLedY, LOW);
    blinkLedR(interval, currMillis);
    break;
    case 2:
    digitalWrite(termLedG, LOW);
    digitalWrite(termLedY, LOW);
    blinkLedR(interval1, currMillis);
    break;
    case 1:
    digitalWrite(termLedG, LOW);
    digitalWrite(termLedY, LOW);
    blinkLedR(interval2, currMillis);
    break;
    }
  } else {
    blinkLedG(interval2, currMillis);
    blinkLedY(interval2, currMillis);
    blinkLedR(interval2, currMillis);
  }
}

void grzalka() {
  if (temp >= tempH)
  {
      digitalWrite(grzalkaRel, LOW); // temperatuta bezpieczna - grzalka wylaczona
      grzalkaStan = 0;
  }
  else if (temp <= tempL)
  {
      digitalWrite(grzalkaRel, HIGH); // temperatura niska - grzalka wlaczona
      grzalkaStan = 1;
  }
  else
  {
    if (grzalkaStan == 0)
    {
      digitalWrite(grzalkaRel, LOW); // temperatuta bezpieczna - grzalka wylaczona
      grzalkaStan = 0;
    }
    else
    {
      digitalWrite(grzalkaRel, HIGH); // temperatura niska - grzalka wlaczona
      grzalkaStan = 1;
    }
  }    
}

void poziomWody() {
  // Przelaczanie diod poziomu wody
  if (poziomL == HIGH && poziomH == LOW ) // poziom za niski, swieci się dolna dioda (czerwona)
  {
    digitalWrite(poziomLedH, LOW);
    digitalWrite(poziomLedN, LOW);
    digitalWrite(poziomLedL, HIGH);
    waterlevel = ("LOW");
  }
  else if (poziomL == LOW && poziomH == LOW ) // poziom właściwy, swieci się środkowa dioda (zielona)
  {
    digitalWrite(poziomLedH, LOW);
    digitalWrite(poziomLedN, HIGH);
    digitalWrite(poziomLedL, LOW);
    waterlevel = ("OK");
  }
  else if (poziomL == LOW && poziomH == HIGH ) // poziom zbyt wysoki, swieci się górna dioda (czerwona)
  {
    digitalWrite(poziomLedH, HIGH);
    digitalWrite(poziomLedN, LOW);
    digitalWrite(poziomLedL, LOW);
    waterlevel = ("HIGH");
  }
  else // swieca sie obie czerwone diody w przypadku awarii czujnikow
  {
    digitalWrite(poziomLedH, HIGH);
    digitalWrite(poziomLedN, LOW);
    digitalWrite(poziomLedL, HIGH);
    waterlevel = ("ERR");
  }
}

void blinkLedG(unsigned int interval, unsigned long currMillisG) {
  if (currMillisG - prevMillisG >= interval) {
  // save the last time you blinked the LED
  prevMillisG = currMillisG;

  // if the LED is off turn it on and vice-versa:
  if (tempLedGstate == LOW) {
    tempLedGstate = HIGH;
  } else {
    tempLedGstate = LOW;
  }

  // set the LED with the ledState of the variable:
  digitalWrite(termLedG, tempLedGstate);
  }
}

void blinkLedY(unsigned int interval, unsigned long currMillisY) {
  if (currMillisY - prevMillisY >= interval) {
  // save the last time you blinked the LED
  prevMillisY = currMillisY;

  // if the LED is off turn it on and vice-versa:
  if (tempLedYstate == LOW) {
    tempLedYstate = HIGH;
  } else {
    tempLedYstate = LOW;
  }

  // set the LED with the ledState of the variable:
  digitalWrite(termLedY, tempLedYstate);
  }
}

void blinkLedR(unsigned int interval, unsigned long currMillisR) {
  if (currMillisR - prevMillisR >= interval) {
  // save the last time you blinked the LED
  prevMillisR = currMillisR;

  // if the LED is off turn it on and vice-versa:
  if (tempLedRstate == LOW) {
    tempLedRstate = HIGH;
  } else {
    tempLedRstate = LOW;
  }

  // set the LED with the ledState of the variable:
  digitalWrite(termLedR, tempLedRstate);
  }
}

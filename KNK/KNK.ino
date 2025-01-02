// KNK v2 - blink feature
//

#include <OneWire.h>
#include <DallasTemperature.h>

// Ustalenie temperatur granicznych
// Histereza od tempL do tempH

  // Temperatura wysoka (grzałka wylaczona)
  uint8_t tempH = 14;   // 14

  // Temperatura ponad przeciętna (grzałka wylaczona)
  uint8_t tempCH = 11;  // 11

  // Temperatura dopuszczalna (grzalka wylaczona)
  uint8_t tempLC = 8;   // 8

  // Temperatura krytyczna (grzalka wlaczona)
  uint8_t tempL = 5;    // 5

  // Temperatura zamarzania (grzalka wlaczona)
  uint8_t tempZ = 2;    // 2

// Piny wyjściowe diod termometru
  const uint8_t termG = 1; // Zielona - temperatura wysoka (pin3)
  const uint8_t termY = 2; // Zolta - temperatura dopuszczalna (pin4)
  const uint8_t termR = 3; // Czerwona - temperatura krytyczna (pin5)
  
// Pin wyjściowy przekaźnika grzałki
  const uint8_t grzalkaRel = 4; // Moduł przekaznika 5V  (pin6)
  uint8_t grzalkaStan = 0; // grzałka wyłączona

// Piny wejściowe dla czujnikow poziomu wody
  const uint8_t poziomLpin = 9; // Czujnik dolny (pin15)
  const uint8_t poziomHpin = 10; // Czujnik górny (pin16)

// Piny wyjściowe diod poziomu wody
  const uint8_t poziomLedH = 5; // Czerwona - poziom wysoki (pin11)
  const uint8_t poziomLedN = 6; // Zielona - poziom odpowiedni (pin12)
  const uint8_t poziomLedL = 7; // Czerwona - poziom niski (pin13)

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

// USTAWIENIA - Początek
void setup(void)
{     
  // Inicjalizacja termometru
    sensors.begin();

  // Ustawienie rozdzielczości termometru
    sensors.setResolution(pipeThermometer, 12);
  
  // Ustawienia dla diod termometru
    pinMode(termG, OUTPUT); // Zielona
    pinMode(termY, OUTPUT); // Zolta
    pinMode(termR, OUTPUT); // Czerwona
  
  // Ustawienia dla przekaznika grzalki
    pinMode(grzalkaRel, OUTPUT); // Przekaznik 5V

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
    digitalWrite(termR, HIGH);
    digitalWrite(termY, HIGH);
    digitalWrite(termG, HIGH);
    digitalWrite(grzalkaRel, HIGH); // do usuniecia
  delay (3000);
    digitalWrite(poziomLedH, LOW);
    digitalWrite(poziomLedN, LOW);
    digitalWrite(poziomLedL, LOW);
    digitalWrite(termR, LOW);
    digitalWrite(termY, LOW);
    digitalWrite(termG, LOW);

  // Wyłączenie grzałki (stan początkowy)
    digitalWrite(grzalkaRel, LOW);
    grzalkaStan = 0; // grzałka wyłączona

}
// USTAWIENIA - Koniec

void loop(void)
{   
  // Odczyt temperatury
  sensors.requestTemperatures(); // Pobieranie temperatury
  float temperatura = sensors.getTempCByIndex(0);
  
  // Odczyt stanu czujników poziomu wody
  uint8_t poziomL = digitalRead(poziomLpin);
  uint8_t poziomH = digitalRead(poziomHpin);

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
  
  // Załączanie grzałki na podstawie histerezy tempLC-tempL
 if (temperatura >= tempCH)
  {
      digitalWrite(grzalkaRel, LOW); // temperatuta bezpieczna - grzalka wylaczona
      grzalkaStan = 0;
  }
  else if (temperatura <= tempL)
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

  
  // Przelaczanie diod w zaleznosci od temperatur granicznych
  if (temperatura <= tempZ) // świecą się wszystkie diody
  {
    digitalWrite(termR, HIGH);
    digitalWrite(termY, HIGH);
    digitalWrite(termG, HIGH);
  }
  else if (temperatura > tempZ && temperatura <= tempL) // świeci się czerwona
  {
    digitalWrite(termR, HIGH);
    digitalWrite(termY, LOW);
    digitalWrite(termG, LOW);
  }
  else if (temperatura > tempL && temperatura <= tempLC) // świeci się czerwona i zolta
  {
    digitalWrite(termR, HIGH);
    digitalWrite(termY, HIGH);
    digitalWrite(termG, LOW);
  }
  else if (temperatura > tempLC && temperatura < tempCH) // świeci się zolta
  {
    digitalWrite(termR, LOW);
    digitalWrite(termY, HIGH);
    digitalWrite(termG, LOW);
  }
  else if (temperatura >= tempCH && temperatura < tempH) // świeci się zolta i zielona
  {
    digitalWrite(termR, LOW);
    digitalWrite(termY, HIGH);
    digitalWrite(termG, HIGH);
 }
  else if (temperatura >= tempH) // świeci się zielona
  {
    digitalWrite(termR, LOW);
    digitalWrite(termY, LOW);
    digitalWrite(termG, HIGH);
  }
  else // świeci się czerwona i zielona w przypadku niezidentyfikowanej temperatury - blad, który nie powinien sie pojawic
  {
    digitalWrite(termR, HIGH);
    digitalWrite(termY, LOW);
    digitalWrite(termG, HIGH);
  }

delay(1000);
}

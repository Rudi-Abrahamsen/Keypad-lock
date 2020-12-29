/*
  The circuit:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)
*/

// include the library code:
#include <LiquidCrystal.h>

const int riktigin = 8;//inputpinen fra kodelåsarduinoen om riktig passord tastes - går fra PIN11 på den andre arduinoen
const int feilin = 9;//inputpinen fra kodelåsarduinoen om feil passord tastes - Går fra PIN12 på andre arduino

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


/*NB - JEG MÅTTE ERKLÆRE FUNKSJONEN HER FORDI ARDUINO IKKE HAR EN PREPROSESSOR SOM SJEKKER ERKLÆRINGER.
ELLERS VILLE JEG HA ERKLÆRT DENNE UNDER LOOP FUNKSJONEN*/

void kodestat()//funksjon for hva som skal skrives på LCD'n
{
  while (digitalRead(riktigin) == HIGH && digitalRead(feilin) == LOW)
  {
    lcd.clear();//fjerner tidligere tekst
    
    lcd.print("Riktig!");
    delay(1000);
  }

  while (digitalRead(riktigin) == LOW && digitalRead(feilin) == HIGH)
  {
    lcd.clear();//fjerner tidligere tekst
    
    lcd.print("Feil!");
    delay(1000);
  }
  
  while (digitalRead(riktigin) == LOW && digitalRead(feilin) == LOW)
  {
    lcd.clear();//fjerner tidligere tekst
    
    lcd.print("Password");
    delay(1000);
  }
  return;
}

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // kjør en startuptest for å vise at LCD fungerer, clearer den etter 1 sekund.
  lcd.print("hello, world!");
  delay(1000);
  lcd.clear();

  pinMode(riktigin, INPUT);//tar imot signal fra keypadarduino om riktig passord
  pinMode(feilin, INPUT);//tar imot signal fra kaypadarduino om feil passord

}

void loop()
{
  lcd.setCursor(0, 1); // setter "cursor" til kolonne 1, rute 0

  kodestat();//funksjonen refereres over "setup" - se egen kommentar
  
  lcd.clear();

}



#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>

//denne må være passordlengde + 1 pga. "NULLpointer"
#define PasswordLength 7

const int riktiglcd = 11;//pin for å gi info til LCD arduino om å si ifra at riktig kode er tastet
const int feillcd = 12;//pin for å gi info til LCD ardunio om å si ifra at kode er feil
const int open = 0;//servoens possisjon får "åpen" - servoposs 0 grader
const int lock = 180;//tilsvarer "open" - men låsepos er 180 grader
char Data[PasswordLength]; //Avgjør lengden på passordet - denne er definert over
char Master[PasswordLength] = "123456"; //Her er passordet til keypaden, men den kan også gjøres lenger eller kortere. Vurder å gjøre dynamisk.
byte datacount = 0, mastercount = 0;//definerer variabler for telling av antall tastetrykk
bool Passisgood;
char customKey;
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};//skisserer opp keypaden for "keypad.h" biblioteket - Dette finnes i dokumentasjonen til biblioteket

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); //Oversetter tastetrykkene til å gi de symbolene tastene viser.


Servo myservo;  // Definerer servoen


/*NB - JEG MÅTTE ERKLÆRE FUNKSJONEN HER FORDI ARDUINO IKKE HAR EN PREPROSESSOR SOM SJEKKER ERKLÆRINGER.
ELLERS VILLE JEG HA ERKLÆRT DENNE UNDER LOOP FUNKSJONEN*/
void clearData()//Egen funksjon for å tømme "data" variablen, så ikke man får feil passord pga missclicks
{
  while (datacount != 0)
  {
    Data[datacount--] = 0;
  }
  return;
}

void setup()
{
  Serial.begin(9600);

  myservo.attach(10);//definerer koblingspinnen til servoen

  //definerer pinmodes for enkel kommunikasjon mellom arduinoer
  pinMode(riktiglcd, OUTPUT);
  pinMode(feillcd, OUTPUT);

  //Setter begge pins til LOW for å unngå bugs
  digitalWrite(riktiglcd, LOW);
  digitalWrite(feillcd, LOW);
}

void loop()
{
  myservo.write(lock);//sørger for at døra holder seg låst
  customKey = customKeypad.getKey();//konstant overvåkning etter tastetrykk - tastene lagres i customKey variablen
  if (customKey)
  {
    Data[datacount] = customKey;
    datacount++;
  }//denne loopen lagrer antall tastetrykk i variablen "datacount" - Dette gjøres hvis et tastetrykk oppdages

  if (datacount == PasswordLength - 1) //når det har vært utført like mange tastetrykk som lengden på passordet - NULLpointer skal de faktiske tastene sammenliknes med passord
  {
    if (!strcmp(Data, Master)) //sammenlikner Data og Master
    {
      myservo.write(open);              // Sett servo i åpen possisjon - Variablen "open" er definert i starten av programmet
      
      //Send signal til LCD om å printe riktig
      digitalWrite(riktiglcd, HIGH);
  	  digitalWrite(feillcd, LOW);
      
      delay(5000);                      // Holder posisjonen i 5 sekunder - denne burde trigges av en føler eller frem til alternativ kode er tastet
     
      clearData();
      digitalWrite(riktiglcd, LOW);//tilbakestill display
      digitalWrite(feillcd, LOW);
      
      myservo.write(lock);              // Sett servo i låst possisjon - Variablen "lock" er definert i starten av programmet
      
      delay(15);                       // holder i 15 millisekunder før neste linje
    }

    else//Dersom passordet er feil, fjernes det helt for et nytt forsøk
    {
      delay(500);
      
      //gir input til LCD for å printe feil
      digitalWrite(riktiglcd, LOW);
  	  digitalWrite(feillcd, HIGH);
      
      delay(5000);
      
      clearData();//denne delen fjerner det feilaktige forsøket på passord
      
      digitalWrite(riktiglcd, LOW);//tilbakestill display
      digitalWrite(feillcd, LOW);
    }

  }

}


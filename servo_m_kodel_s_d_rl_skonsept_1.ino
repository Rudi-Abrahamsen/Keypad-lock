#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>

//denne må være passordlengde + 1 pga. "NULLpointer"
#define PasswordLength 7


const int open = 180;//servoens possisjon får "åpen" - servoposs 180 grader
const int lock = 0;//tilsvarer "open" - men låsepos er 0 grader
char Data[PasswordLength]; //Avgjør lengden på passordet - denne er definert over
char Master[PasswordLength] = "123456"; //Her er passordet til keypaden, men den kan også gjøres lenger eller kortete. Relativt lett å hacke om man har tilgang på Arduinoen.
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


Servo myservo;  // lager servoen i programmet



void setup() 
{
  Serial.begin(9600);
  
  myservo.attach(10);//definerer koblingspinnen til servoen
}

void loop() 
{
  customKey = customKeypad.getKey();//konstant overvåkning etter tastetrykk - tastene lagres i customKey variablen
  if (customKey)
  {
    Data[datacount] = customKey; 
    datacount++; 
   }//denne loopen lagrer antall tastetrykk i variablen "datacount" - Dette gjøres hvis et tastetrykk oppdages
  
  if(datacount == PasswordLength-1)//når det har vært utført like mange tastetrykk som lengden på passordet - NULLpointer skal de faktiske tastene sammenliknes med passord
  {
    if(!strcmp(Data, Master))//sammenlikner Data og Master
    {
    myservo.write(open);              // Sett servo i åpen possisjon - Variablen "open" er definert i starten av programmet
    delay(5000);                      // Holder posisjonen i 5 sekunder - denne burde trigges av en føler eller frem til alternativ kode er tastet
      
    myservo.write(lock);              // Sett servo i låst possisjon - Variablen "lock" er definert i starten av programmet
    delay(15);                       // holder i 15 millisekunder før neste linje
    }
    
    else//her kjøres clearDate funksjonen dersom passordet ikke stemmer
    {
     delay(1000);
    }
    
    clearData(); 
    
  }
    
}

void clearData()
{
  while(datacount !=0)
  {
    Data[datacount--] = 0; 
  }
  return;
}
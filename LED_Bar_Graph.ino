const int analogPin= A1; // pin of center pin of the potentiometer
const int ledCount= 8; // num of LED
int ledPins[]= {2, 3, 4, 5, 6, 7, 8, 9};

void setup() 
{
  for (int thisLed= 0; thisLed < ledCount; thisLed++)
  {
    pinMode(ledPins[thisLed], OUTPUT);
  }  
}

void loop() 
{
  int sensorReading= analogRead(analogPin);// pwede rekta nalng sa () ay A1
  int ledLevel = map(sensorReading, 0, 1023, 0, ledCount);// i mamap yung 0-1023 to 0-8 na LED
  for(int thisLed = 0; thisLed < ledCount; thisLed++)
 {
   if(thisLed < ledLevel)
   {
     digitalWrite(ledPins[thisLed], HIGH);
   }
   else
   {
     digitalWrite(ledPins[thisLed], LOW);
   }
 }
}


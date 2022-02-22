const int pinIRd = 9;
int IRvalueD = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(pinIRd,INPUT);
}

void loop()
{
  IRvalueD = digitalRead(pinIRd);
  
  if (IRvalueD == 1)
  {
    Serial.print("\t Line is detected, thus the value is : ");
    Serial.println(IRvalueD);
    delay(1000);
  }
  else
  {
    Serial.print(" No line is detected, the value is : ");
    Serial.println(IRvalueD);
    delay(1000);
  }    
}


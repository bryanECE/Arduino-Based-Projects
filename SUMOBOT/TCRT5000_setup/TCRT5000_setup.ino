const int pinIRd = 8;
const int pinIRa = A0;
int IRvalueA = 0;
int IRvalueD = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(pinIRd,INPUT);
  pinMode(pinIRa,INPUT);

}

void loop()
{
  Serial.print("Analog Reading= ");
  Serial.print(IRvalueA);
  Serial.print("\t Digital Reading= ");
  Serial.println(IRvalueD);
  delay(1000);
  
  IRvalueA = analogRead(pinIRa);
  IRvalueD = digitalRead(pinIRd);
    
}

// Kapag nadetect yung black mataas value ng analog and yung digital value ay 1 kase inaabsord yung IR
// 

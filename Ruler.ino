
void setup() 
{
  Serial.begin (9600);
  pinMode(13, OUTPUT);
  pinMode(12, INPUT);
}
void loop() 
{
  long duration, distance;
  digitalWrite(13, LOW);        
  delayMicroseconds(2);              
  digitalWrite(13, HIGH);
  delayMicroseconds(10);           
  digitalWrite(13, LOW);
  duration = pulseIn(12, HIGH);
  distance = (duration/2) / 29.1;
  if (distance >= 200 || distance <= 0)
  {
    Serial.println("Out of range");
  }
  else 
  {
    Serial.print(distance);
    Serial.println(" cm");
  }
  delay(500);
}

/*
Instrululu Project 
*/

float gasLevel = 0;                       
int relay = 9;             
int buzzer = 8;
int mini= 10;  // mini fan

void setup()
{
  Serial.begin(9600);                     
  Serial.println("Instrumentation Project");
  Serial.println("-------ECE135P-------");
  Serial.println("Arduino-based Air Quality Control System");
  
  pinMode(A0,INPUT);                
  pinMode(relay,OUTPUT);          
  pinMode(buzzer,OUTPUT);
  pinMode(mini,OUTPUT);
  
}
void loop() 
{
// Para sa Serial Monitor ppm value ng gas
  gasLevel = analogRead(A0);
  float voltage = gasLevel * (5.0 / 1023.0);
  Serial.println("-----------------------------");
  
  Serial.print("Gas Level: ");
  Serial.println(gasLevel,DEC);
  Serial.print("Voltage Level: ");
  Serial.println(voltage);
  delay(1000);         
  
  if(gasLevel<200)    // Assumed vaue, near theoretical
  {
    digitalWrite(relay, LOW);  // big fan
    digitalWrite(mini, LOW);  // mini fan
  }
  else if (gasLevel >200 && gasLevel<300)
  {
    digitalWrite(relay,HIGH);
    digitalWrite(mini,LOW);    
  }
  else if (gasLevel >300)
  {
    digitalWrite(relay,HIGH); 
    digitalWrite(mini,HIGH);    
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    delay(50);
  } 
 }

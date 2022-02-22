enum blinkStates 
{
  BLINK_DIS, // blink disable
  BLINK_EN, // blink enable
  LED_ON, // we want the led to be on for interval
  LED_OFF // we want the led to be off for interval
};
 
enum blinkStates ledState;
 
void setup() 
{
  delay(1000);
  Serial.begin(9600);
  
  Serial.println(F("---------"));
  Serial.print(F("BLINK_DIS = "));
  Serial.println(BLINK_DIS);
 
  Serial.print(F("BLINK_EN = "));
  Serial.println(BLINK_EN);
 
  Serial.print(F("LED_ON = "));
  Serial.println(LED_ON);
 
  Serial.print(F("LED_OFF = "));
  Serial.println(LED_OFF);
}
 
void loop()
{ 
  // no need since it is only for checking
}

int led= 12; // initializing that led blinking happens in PIN13
void setup() { 
  pinMode(led, OUTPUT); // notes that pin 13 will be an output
}
void loop() {
  digitalWrite(led, HIGH);  // High , connected to 5V 
  delay(100); // millisecond                     
  digitalWrite(led, LOW);   // Low, coonects to ground , to turn LED off
  delay(100);                       
}// connect in series the 3V buzzer with resistor 200kohms and red led to also see the effects of the delay

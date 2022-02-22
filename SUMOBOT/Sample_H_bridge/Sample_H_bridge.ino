// 4-WD
#define inOne 4           //IN1
#define speedPinA 5       //ENA
#define inTwo 7           //IN3
#define speedPinB 6       //ENB
const int revUno = 2;     //IN2
const int revDos = 3;     //IN4
int Speed=128;

void setup() 
{
  Serial.begin(9600);
  pinMode(inTwo, OUTPUT);
  pinMode(speedPinB, OUTPUT);
  pinMode(inOne, OUTPUT);
  pinMode(speedPinA, OUTPUT);
}

void loop() 
{
  
  startCar();
  delay(1000);


  leftTurn();
  delay(1000);
}
// ========================================== MOTOR MOVEMENT ==================================
void startCar()
{ 

  digitalWrite(inOne, 1);
  digitalWrite(inTwo, 1); 
  digitalWrite(revUno, 0);
  digitalWrite(revDos, 0);
  analogWrite(speedPinA, Speed);
  analogWrite(speedPinB, Speed); 
}

void reverseCar()
{
  digitalWrite(inOne, 0);
  digitalWrite(inTwo, 0); 
  digitalWrite(revUno, 1);
  digitalWrite(revDos, 1);
  analogWrite(speedPinA, Speed);
  analogWrite(speedPinB, Speed);
}

void leftTurn()
{
  digitalWrite(inOne, 1);         //right forward
  digitalWrite(inTwo, 0);
  digitalWrite(revUno, 0);
  digitalWrite(revDos, 1);        // left reverse
  analogWrite(speedPinA, Speed); 
  analogWrite(speedPinB, Speed); 
}

void rightTurn()
{
  digitalWrite(inOne, 0);
  digitalWrite(inTwo, 1);         // left right
  digitalWrite(revUno, 1);        // right reverse
  digitalWrite(revDos, 0);
  analogWrite(speedPinA, Speed);
  analogWrite(speedPinB, Speed);
}

void breakCar()
{
  digitalWrite(inOne, 0);
  digitalWrite(inTwo, 0);
  analogWrite(speedPinA, 0); 
  analogWrite(speedPinB, 0);
} 

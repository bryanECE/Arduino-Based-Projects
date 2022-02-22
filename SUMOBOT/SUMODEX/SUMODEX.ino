#include <NewPing.h>

enum State
{
  stateScanning,            
  stateWander,                
  stateRotate2Target,       
  stateChargeTarget,         
  stateEdgeRecovery         
};

//IR sensor
const int firstpinIR = 8;
int firstIR = 0;
const int secondpinIR = 9;
int secondIR = 0;

// 4-WD
#define inOne 4      
#define speedPinA 5
#define inTwo 7
#define speedPinB 6
const int revUno = 2;
const int revDos = 3;
int Speed=250;

//ULTRASONIC
#define trigPin 12  
#define echoPin 11  
#define maxDistance 100 
NewPing sonar(trigPin, echoPin, maxDistance);

State currentState = stateScanning;        

void setup() 
{
  Serial.begin(9600);
  
  pinMode(firstpinIR,INPUT);
  pinMode(secondpinIR,INPUT);

  pinMode(inTwo, OUTPUT);
  pinMode(speedPinB, OUTPUT);
  pinMode(inOne, OUTPUT);
  pinMode(speedPinA, OUTPUT);
}

void loop() 
{
  switch (currentState) 
  {
    case stateScanning:
      {
        unsigned int uS = sonar.ping();
        int pingReading = sonar.convert_cm(uS);
        Serial.print("Ping: ");
        Serial.print(pingReading); 
        Serial.println("cm");
        
        if(pingReading <= 55) 
        {
          currentState=stateRotate2Target;
        } 
        else 
        {
          currentState=stateWander;
        }
      }
      break;

   case stateRotate2Target:
      {

        delay(15);
        breakCar();
        delay(50); 
        unsigned int uS = sonar.ping();
        int pingReading = sonar.convert_cm(uS);  
        while(pingReading > 55)
        {
          leftTurn();
          delay(20);
        }
        Serial.println("Rotate Mode Done!!! (breaks are enabled, ready to charge)"); 
        currentState=stateChargeTarget;
      }
      break;

    case stateChargeTarget:
      {
        Serial.println("CHARGING!!!");
        Serial.println("CHARGING!!!");
              do
              {
                  startCar();
                  delay(20);     
                  firstIR = digitalRead(firstpinIR);
                  secondIR = digitalRead(secondpinIR); 
              }
              while(firstIR == 0 && secondIR == 0);

        breakCar();
        currentState=stateEdgeRecovery;
      }
      break;
      
    case stateEdgeRecovery:
      {
        firstIR = digitalRead(firstpinIR);
        secondIR = digitalRead(secondpinIR);
                    
              if (firstIR == 1)
              {
                Serial.print("\t IR1 triggered, thus the value is : ");
                Serial.println(firstIR);
                delay(1000);
                
                reverseCar();
                delay(100);
                breakCar();
                delay(50);
                rightTurn();
                delay(500);
              }
              
              else if (secondIR == 1)
              {
                Serial.print("\t IR2 triggered, thus the value is : ");
                Serial.println(secondIR);
                delay(1000);
                
                startCar();
                delay(200);
                breakCar();
                delay(50);
              }
              
              else
              {
                Serial.println(" No line is detected");
                delay(1000);
              }    
              
        currentState=stateScanning;
      }
      break;

    case stateWander:
      {
        firstIR  =  digitalRead(firstpinIR);
        secondIR =  digitalRead(secondpinIR);

        Serial.println("-----> WANDERING!!! <-----");
        rightTurn();     
        delay(20);
      
          if(firstIR == 1 || secondIR == 1) 
              {
                currentState=stateEdgeRecovery;
              } 
          else if(firstIR != 1 && secondIR !=1)   
              {
                currentState=stateScanning;
              } 
      }
      break;
  }   
}

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
  digitalWrite(inOne, 1);         
  digitalWrite(inTwo, 0);
  digitalWrite(revUno, 0);
  digitalWrite(revDos, 1);        
  analogWrite(speedPinA, Speed); 
  analogWrite(speedPinB, Speed); 
}

void rightTurn()
{
  digitalWrite(inOne, 0);
  digitalWrite(inTwo, 1);         
  digitalWrite(revUno, 1);       
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

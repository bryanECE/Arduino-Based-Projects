#include <NewPing.h>

enum State
{
  stateScanning,            // Robot scans from static position.
  stateWander,              // Robot wanders around.
  stateRotate2Target,       // Assumes target is found, will rotate to largest prox value
  stateChargeTarget,        // Charges a target.
  stateEdgeRecovery         // Recovers from finding an edge.
};

// Global Variables
// check lahat ng pins first, try all pins for H-bridge is pwm

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
int Speed = 100;

//ULTRASONIC
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 100
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Declare and initialize the state variable of type State
State currentState = stateScanning;

void setup()
{
  Serial.begin(9600);

  pinMode(firstpinIR, INPUT);
  pinMode(secondpinIR, INPUT);

  pinMode(inTwo, OUTPUT);
  pinMode(speedPinB, OUTPUT);
  pinMode(inOne, OUTPUT);
  pinMode(speedPinA, OUTPUT);
}

void loop() {

  switch (currentState)
  {
    //-------------------------------------------------------------------------------
    case stateScanning:
      {
        unsigned int uS = sonar.ping();
        int pingReading = sonar.convert_cm(uS);
        Serial.print("Ping: ");
        Serial.print(pingReading);
        Serial.println("cm");

        //Calibrate total length of arena

        if (pingReading <= 55)
        {
          currentState = stateRotate2Target;
        }
        else
        {
          currentState = stateWander;
        }
      }
      break;
    //-------------------------------------------------------------------------------
    case stateRotate2Target:
      {
        delay(15);
        breakCar();
        delay(50);
        //bagong add
        unsigned int uS = sonar.ping();
        int pingReading = sonar.convert_cm(uS);

        /*
        if (pingReading > 55)
        {
          leftTurn();
          delay(20);
        }
        */

        Serial.println("Rotate Mode Done!!! (breaks are enabled, ready to charge)");
        currentState = stateChargeTarget;
      }
      break;
    //-------------------------------------------------------------------------------
    case stateChargeTarget:
      {
        Serial.println("CHARGING!!!");
        Serial.println("CHARGING!!!");

        //startCar();

        // add delay or to remove it, or else it will go through the line without recognizing the edge recovery
        // try doing a for statement include also if IR reading is 1 then it goes to current state recovery

        do
        {
          int Speed= 250;
          startCar();
          delay(100);
          firstIR = digitalRead(firstpinIR);
          secondIR = digitalRead(secondpinIR);

          /*
          Serial.print("Front IR Reading: ");
          Serial.println(firstIR);

          Serial.print("Back IR Reading: ");
          Serial.println(secondIR);
          */
        }
        while (firstIR == 0 && secondIR == 0);

        breakCar();
        currentState = stateEdgeRecovery;
      }
      break;
    //-------------------------------------------------------------------------------
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

          //leftTurn();
          //delay(20);

        }

        else if (secondIR == 1)
        {
          Serial.print("\t IR2 triggered, thus the value is : ");
          Serial.println(secondIR);
          delay(1000);

          startCar();
          delay(500);
          breakCar();
          delay(50);
        }

        else
        {
          Serial.println(" No line is detected");
          delay(1000);

          //currentState=stateScanning;
        }
        currentState = stateScanning;
      }
      break;
    //-------------------------------------------------------------------------------
    case stateWander:
      {
        firstIR  =  digitalRead(firstpinIR);
        secondIR =  digitalRead(secondpinIR);

        Serial.println("-----> WANDERING!!! <-----");
        rightTurn();      // if nasa left side yung bot
        delay(20);

        //leftTurn();
        //delay(20);

        if (firstIR == 1 || secondIR == 1)
        {
          currentState = stateEdgeRecovery;
        }
        else if (firstIR != 1 && secondIR != 1) // include na if yung ultrasonic sense is below threshold ibig sabihin may nasend siya
        {
          currentState = stateScanning;
          //currentState=stateRotate2Target;
        }

        //rightTurn();
        //delay(100);
        //rightTurn();
        //delay(100);
        // Add code for wander routine
      }
      break;
      //-------------------------------------------------------------------------------
      //-------------------------------------------------------------------------------
  }
  // End of the switch statement
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

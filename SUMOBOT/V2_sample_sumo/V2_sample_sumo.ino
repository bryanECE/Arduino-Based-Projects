// INITIALIZATION:
#define MOTOR_1_DIRECTION         8       // Motor control HIGH = FWD, LOW = BWD
#define MOTOR_1_SPEED             9       // Speed inputt
#define MOTOR_2_SPEED            10       // Speed inpu
#define MOTOR_2_DIRECTION        11       // Motor control HIGH = FWD, LOW = BWD

//LIBRARIES
#include <NewPing.h>        
#include <SM.h>

//ULTRASONIC PINS
#define TRIGGER_PIN  12
#define ECHO_PIN      3

//MAXIMUM DISTANCE WE WANT TO PING IN CM (TO 500 CM)
#define MAX_DISTANCE 100  

//SERVO PIN
#define SERVO_PIN    6

//OPPONENTS "FOUND" INDICATOR
#define OPPONENT_FOUND_INDICATOR    13

//----( "OPPONENTS" DIRECTIONS (index into TargetArray )---------
#define TARGET_FOUND_ANY     0   // Values will be "true" or "false"
#define TARGET_LEFT          1
#define TARGET_LEFT_CENTER   2
#define TARGET_CENTER        3
#define TARGET_RIGHT_CENTER  4
#define TARGET_RIGHT         5
#define TARGET_ARRAY_SIZE    6

////OPPONENTS "FOUND" DISTANCE 
#define TARGET_TOO_CLOSE     30

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);       // Set pins and maximum distance.
SM RoboGo(&RoboStartState);                                //create simple statemachine

// VARIABLES
unsigned int uS;         // Result of a ping: MicroSeconds
unsigned int cm;         // Distance calculated for ping (0 = outside set distance range)
unsigned int cm_now;     // For test

int TargetArray[TARGET_ARRAY_SIZE];            // Holds the directions a Target was found in
int DirectionsToLook = 3;                      // For LookAround()
int ServoDirectionData[3] = {
  2500, 1600, 600};

int  LEFT_LINE_SENSOR = 5;      //left
int  RIGHT_LINE_SENSOR = 7;     //right

// INPUTS AND OUTPUTS
void setup(){
  Serial.begin(9600);  
  pinMode(MOTOR_1_DIRECTION, OUTPUT);
  pinMode(MOTOR_1_SPEED, OUTPUT);
  pinMode(MOTOR_2_DIRECTION, OUTPUT);
  pinMode(MOTOR_2_SPEED, OUTPUT);
  pinMode(SERVO_PIN,OUTPUT);  
  pinMode(OPPONENT_FOUND_INDICATOR,OUTPUT);    
}
int toggle = 0;
int toggle2= 0;
boolean normal = true;
boolean machine = true;

// MAIN PROGRAM ROUTINE
void loop()                     
{
  LEFT_LINE_SENSOR = digitalRead(5);//left
  RIGHT_LINE_SENSOR = digitalRead(7);//right

  if(LEFT_LINE_SENSOR == 0){ 
    BACKWARD_SLOW(1000);
    SPIN_LEFT(500);
    normal=true;
  }
  if(RIGHT_LINE_SENSOR == 0){ 
    BACKWARD_SLOW(1000);
    SPIN_RIGHT(500);
    normal=true;
  }
  else if(normal == true){
    FORWARD_SLOW();
    STOP();

    machine=true;
    toggle2=0;
  }
  if(machine == true){
    toggle2=0;
    EXEC(RoboGo);                 //run statemachine
    delay(100);
  }
}
/*---------------------------*/
void RUN_BOT(int spd1,int spd2, boolean direction )
{
  digitalWrite(MOTOR_1_DIRECTION,direction);
  digitalWrite(MOTOR_2_DIRECTION,direction);
  analogWrite(MOTOR_2_SPEED, spd1);
  analogWrite(MOTOR_1_SPEED, spd2); 
}
/*---------------------------*/
void FORWARD(void)
{
  digitalWrite(MOTOR_1_DIRECTION,HIGH);
  digitalWrite(MOTOR_2_DIRECTION,HIGH);
}    
/*---------------------------*/
void ROTATE_BOT(int speed, boolean direction)
{
  digitalWrite(MOTOR_1_DIRECTION,direction);
  digitalWrite(MOTOR_2_DIRECTION,~direction);
  analogWrite(MOTOR_1_SPEED, speed);
  analogWrite(MOTOR_2_SPEED, speed); 
}
/*---------------------------*/
void STOP(void)
{
  analogWrite(MOTOR_1_SPEED, 0);
  analogWrite(MOTOR_2_SPEED, 0); 
}
/*---------------------------*/
void FORWARD_SLOW()
{
  digitalWrite(8, HIGH); 
  digitalWrite(9, 128);
  digitalWrite(10, 128);
  digitalWrite(11, HIGH);
}
/*---------------------------*/
void FORWARD_MEDIUM()
{
  digitalWrite(8, HIGH); 
  digitalWrite(9, 200);
  digitalWrite(10, 200);
  digitalWrite(11, HIGH);
}
/*---------------------------*/
void FORWARD_FAST()
{
  digitalWrite(8, HIGH); 
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
}
/*---------------------------*/
void BACKWARD_SLOW(int HowMuch)
{
  digitalWrite(8, LOW); 
  digitalWrite(9, HowMuch);//run2
  digitalWrite(10, HowMuch);//run1
  digitalWrite(11, LOW);
  delay(1000);
}
/*---------------------------*/
void BACKWARD_MEDIUM(int HowMuch)
{
  digitalWrite(8, LOW); 
  digitalWrite(9, HowMuch);
  digitalWrite(10, HowMuch);
  digitalWrite(11, LOW);
}
/*---------------------------*/
void BACKWARD_FAST(int HowMuch)
{
  digitalWrite(8, LOW); 
  digitalWrite(9, HowMuch);
  digitalWrite(10, HowMuch);
  digitalWrite(11, LOW);
}
/*---------------------------*/
void SPIN_LEFT(int HowMuch)
{
  digitalWrite(8, LOW); 
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  delay(HowMuch);
}
/*---------------------------*/
void SPIN_RIGHT(int HowMuch)
{
  digitalWrite(8, HIGH); 
  digitalWrite(9, HowMuch);
  digitalWrite(10, HowMuch);
  digitalWrite(11, LOW);
  delay(HowMuch);
}
/*---------------------------*/
unsigned int PingBlink()
{
  uS = sonar.ping();           // Send ping, get ping time in microseconds (uS).
  cm = uS / US_ROUNDTRIP_CM;   // Convert ping time to distance in cm 
  Serial.print("  cm = ");   
  Serial.print(cm,DEC);  

  if ((cm < 80) && (cm != 0))
  {
    return(cm);
  }
  else
  {
    return(100);  // No Valid Distance
  }
}
/*---------------------------*/
void PointServo(int ServoAngle)
{
  for (int i=0;i<20;i++)  // Send the pulse 10 times 
  {
    digitalWrite(SERVO_PIN,HIGH);
    delayMicroseconds(ServoAngle);
    digitalWrite(SERVO_PIN,LOW);
    delay(20);
  }
}
/*---------------------------*/
void LookAround()                              // Sets next state if Target Found
{
  for(int Direction = 0; Direction < DirectionsToLook ; Direction ++)
  {
    Serial.print("DIRECTION = ");     
    Serial.print(Direction,DEC);
    PointServo(ServoDirectionData[Direction]); // Get servo pulse width from array
    delay(200);
    cm_now = PingBlink();    // Read the Ultrasonic distance

    Serial.print(" cm_now = ");     
    Serial.println(cm_now,DEC);    
    if (cm_now < TARGET_TOO_CLOSE) digitalWrite(OPPONENT_FOUND_INDICATOR,HIGH);    
    if (cm_now < 80)
    {
      TargetArray[TARGET_FOUND_ANY ] = true;
    }
    else TargetArray[TARGET_FOUND_ANY ] = false;
    if ((cm_now < TARGET_TOO_CLOSE) && (Direction == 0)) //LEFT
    {
      TargetArray[TARGET_LEFT ] = true;
      Serial.println("e-Bot_potential_target_on_left");  
    }    
    if ((cm_now < TARGET_TOO_CLOSE) && (Direction == 1)) //CENTER
    {
      TargetArray[TARGET_CENTER ] = true;
      Serial.println("e-Bot_potential_target_on_Center");       
    }    
    if ((cm_now < TARGET_TOO_CLOSE) && (Direction == 2)) //RIGHT
    {
      TargetArray[TARGET_RIGHT ] = true;
      Serial.println("e-Bot_potential_target_on_Right");       
    }    
  }
  FORWARD_SLOW();
}

/**************************( STATE MACHINE FUNCTIONS )******************************/
State RoboStartState()
{
  Serial.println("e-Bot_START_Look_State");
  digitalWrite(OPPONENT_FOUND_INDICATOR,LOW);    // LED Means Target Too Close
  FORWARD_FAST();                                // Start moving forward
  delay(25);
  FORWARD_SLOW();                                // Start moving forward
  delay(600);
  RoboGo.Set(RoboSTOPLookState);    
}
/**********************************************************************************/
State RoboSTOPLookState()
{
  Serial.println("e-Bot_STOP_Look_State");
  for (int i = 0; i < TARGET_ARRAY_SIZE; i++) TargetArray[i] = false;
  STOP();                       // Start moving forward
  LookAround();                 // Ping Ultrasonic in different directions, Set TargetArray
  if      (TargetArray[TARGET_CENTER ] == true)  RoboGo.Set(eboTargetCenter);
  else if (TargetArray[TARGET_LEFT ]   == true)  RoboGo.Set(eboTargetLeft);
  else if (TargetArray[TARGET_RIGHT ]  == true)  RoboGo.Set(eboTargetRight);
  else     toggle2==1;         // RoboGo.Set(RoboStartState);  
}
/**********************************************************************************/
State eboTargetLeft()
{
  Serial.println("e-Bot_found_opponent_on_left");
  SPIN_RIGHT(500);
  FORWARD_FAST();
  BACKWARD_FAST(255);
  RoboGo.Set(RoboStartState);  
}
/**********************************************************************************/
State eboTargetCenter()  
{
  Serial.println("e-Bot_found_opponent_on_center");  
  FORWARD_FAST();
  BACKWARD_FAST(255);
  RoboGo.Set(RoboStartState); 
} // END State RoboTargetCenter -----------------
/**********************************************************************************/
State eboTargetRight()
{
  Serial.println("e-Bot_found_opponent_on_right");   
  SPIN_LEFT(500);

  FORWARD_FAST();
  BACKWARD_FAST(255);
  RoboGo.Set(RoboStartState); 
}
// PROGRAM ENDS AND LOOP BACK FOREVER                                                                                                                                                   // PARZ
/**********************************************************************************/





/*
   e-Gizmo eBOT SUMO fighting v1
 (with US-100 sensor and line avoidance)
 
 Using State Machine library of Arduino.
 For eBOT sample code
 
 Modified by Amoree
 @e-Gizmo Mechatronix Central
 Feb 2, 2016
 
 Default Motor pin (PBOT Board):
 M1DIR pin 11 LEFT motor
 M1RUN pin 10
 M2RUN pin 9
 M2DIR pin 8 RIGHT motor
 
 Line pins
 LS1 left pin 7
 LS2 center pin 6
 LS3 right pin 5
 
 Collision pins
 COL1 left pin 4
 COL2 center pin 3
 COl3 right pin 2
 
 US-100 pin
 Trigger pin 12
 echo pin 3 //1
 
 Servo pin
 Output pin 6 //3
 
 */
#include <NewPing.h>          // Runs the Ultrasonic Distance Sensor#include <SM.h>;
#include <SM.h>
#include <State.h>

// Ultrasonic Sensor Pins
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     4  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
// Servo
#define SERVO_PIN    3

// The "Look Around" servo
#define PIN13_LED    13  // The onboard LED

//----( "TARGET FOUND" DIRECTIONS (index into TargetArray )---------
#define TARGET_FOUND_ANY     0   // Values will be "true" or "false"
#define TARGET_LEFT          1
#define TARGET_LEFT_CENTER   2
#define TARGET_CENTER        3
#define TARGET_RIGHT_CENTER  4
#define TARGET_RIGHT         5

#define TARGET_ARRAY_SIZE    6

#define TARGET_TOO_CLOSE     14

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Set pins and maximum distance.
SM RoboGo(&RoboStartState);//create simple statemachine (& is added to remove the error)

/*-----( Declare Variables )-----*/
unsigned int uS;  // Result of a ping: MicroSeconds
unsigned int cm;  // Distance calculated for ping (0 = outside set distance range)
unsigned int cm_now; // For test

int TargetArray[TARGET_ARRAY_SIZE];  // Holds the directions a Target was found in
int DirectionsToLook = 3;  // For LookAround()
int ServoDirectionData[3] = { 
  2500, 1600, 600};

int  col1= 2;
int  col2= 3;
int  col3= 4;
int  ls1 = 5;//left
int  ls2 = 6;//center
int  ls3 = 7;//right
int  m2dir = 8;
int m2run = 9;
int m1dir = 11;
int m1run = 10;
long int rtc;

// Initialization routine
void setup(){
  Serial.begin(9600);  
  pinMode(m2dir, OUTPUT);
  pinMode(m2run, OUTPUT);
  pinMode(m1dir, OUTPUT);
  pinMode(m1run, OUTPUT);
  pinMode(SERVO_PIN,OUTPUT);  
  pinMode(PIN13_LED,OUTPUT);    
  //pinMode(13, OUTPUT);
  rtc=millis()+10;
}

int  colsense = 0;
int  linesense=0;
int giveup=0;
int lastsense;
int runspeed=90;

//timers
byte  retry_dly=0;
byte  ledflsh=25;

int toggle = 0;
int toggle2= 0;
boolean normal = true;
boolean machine = true;

// Main Line following program loop

void loop()                     
{
  toggle2=1;  
  // Hardware Timer service
  if(millis()>=rtc)
  {
    rtc=rtc+10;
    if(retry_dly>0) retry_dly--;
    if(ledflsh>0)
    {
      ledflsh--;
      if(ledflsh==0)
      {
        ledflsh=25;
        PORTB ^= 0x20;
      }
    }                
  }      

  // read the status of colision and line sensors
  colsense=0;
  linesense=0;
  if(digitalRead(col1)==LOW) colsense=1;
  if(digitalRead(col2)==LOW) colsense=colsense+2;
  if(digitalRead(col3)==LOW) colsense=colsense+4;
  if(digitalRead(ls1)==LOW) linesense=1;
  if(digitalRead(ls2)==LOW) linesense=linesense+2;
  if(digitalRead(ls3)==LOW) linesense=linesense+4;


  // if no line is detected (all high)
  if((linesense==0) & (retry_dly==0))
  {
    if(giveup<10)
    {
      //reverse for 20mS
      if(lastsense==1) runBot(runspeed*15/10,runspeed,LOW);
      if(lastsense==3) runBot(runspeed*12/10,runspeed,LOW);
      if(lastsense==4) runBot(runspeed,runspeed*15/10,LOW);
      if(lastsense==6) runBot(runspeed,runspeed*12/10,LOW);

      delay(40);
      giveup++;
    }
    else
      Stop();
    //delay(1000);
  }
  // if line is detected
  if(linesense!=0 && toggle == 0)
  {
    Stop();
    delay(100);
    BackwardFast(225);
    delay(500);
    SpinLeft(980);
    toggle = 1;

    normal = true;
  }    
  else if(linesense!=0 && toggle == 1)
  {
    BackwardFast(225);
    delay(1000);
    SpinRight(980);
    toggle = 0;

    normal = true;
  }   

  // extreme left

  else if(linesense==1)
  {
    BackwardFast(225);
    delay(1000);
    SpinRight(500);
    normal = true;
  }

  // centered

  else if((linesense==2)|(linesense==7))
  {
    BackwardFast(255);
    delay(1000);
    normal = true;
  }      

  // skewed left
  // if(linesense==3)
  // {
  // }
  // extreme right
  // if(linesense==4)
  // {

  // }
  //EXEC(RoboGo);//run statemachine
  //delay(100);
  // skewed right
  else if(linesense==6)
  {
    SpinLeft(255);

    normal =true;
  }
  //singit ko lng para sa one by one line
  ls1 = digitalRead(5);//left
  ls2 = digitalRead(6);//center
  ls3 = digitalRead(7);//right
  if(ls1 == 0){ 
  
    SpinLeft(980);
    normal=true;
  }
  //    if(ls2 == 0){ 
  //  BackwardSlow(980);
  //  normal=true;
  //  }
  if(ls3 == 0){ 
  
    SpinRight(980);
    normal=true;
  }
  else if(normal == true){

    ForwardSlow();
    //delay(1000);
    //Stop();

    machine=true;
    toggle2=0;

  }
  if(machine == true){
    toggle2=0;
    EXEC(RoboGo);//run statemachine
    delay(100);
  }
}

void runBot(int spd1,int spd2, boolean direction )
{
  digitalWrite(m2dir,direction);
  digitalWrite(m1dir,direction);
  analogWrite(m1run, spd1);
  analogWrite(m2run, spd2); 
}

void dirfwd(void)
{
  digitalWrite(m2dir,HIGH);
  digitalWrite(m1dir,HIGH);
}    

void rotBot(int speed, boolean direction)
{
  digitalWrite(m2dir,direction);
  digitalWrite(m1dir,~direction);
  analogWrite(m2run, speed);
  analogWrite(m1run, speed); 
}

void Stop(void)
{
  analogWrite(m2run, 0);
  analogWrite(m1run, 0); 
}

//---MOTOR CONTROL ----
void ForwardSlow()
{
  digitalWrite(8, HIGH); 
  digitalWrite(9, 128);
  digitalWrite(10, 128);
  digitalWrite(11, HIGH);

}
/*---------------------------*/

void ForwardMedium()
{
  digitalWrite(8, HIGH); 
  digitalWrite(9, 200);
  digitalWrite(10, 200);
  digitalWrite(11, HIGH);
  ; 
}
/*---------------------------*/
void ForwardFast()
{
  digitalWrite(8, HIGH); 
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
}
/*---------------------------*/
void BackwardSlow(int HowMuch)
{
  digitalWrite(8, LOW); 
  digitalWrite(9, HowMuch);//run2
  digitalWrite(10, HowMuch);//run1
  digitalWrite(11, LOW);
  delay(1000);
}
/*---------------------------*/
void BackwardMedium(int HowMuch)
{
  digitalWrite(8, LOW); 
  digitalWrite(9, HowMuch);
  digitalWrite(10, HowMuch);
  digitalWrite(11, LOW);
}
/*---------------------------*/
void BackwardFast(int HowMuch)
{
  digitalWrite(8, LOW); 
  digitalWrite(9, HowMuch);
  digitalWrite(10, HowMuch);
  digitalWrite(11, LOW);
}
/*---------------------------*/
/*void Stop()
 {
 digitalWrite(8, HIGH); 
 digitalWrite(9, LOW);
 digitalWrite(10, LOW);
 digitalWrite(11, HIGH); 
 }
/*---------------------------*/
void SpinLeft(int HowMuch)
{
  digitalWrite(8, LOW); 
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  delay(HowMuch);
  //Stop();
}
/*---------------------------*/
void SpinRight(int HowMuch)
{
  digitalWrite(8, HIGH); 
  digitalWrite(9, HowMuch);
  digitalWrite(10, HowMuch);
  digitalWrite(11, LOW);
  delay(HowMuch);
  //Stop();
}
/*---------------------------*/
unsigned int PingBlink()
{
  uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  cm = uS / US_ROUNDTRIP_CM; // Convert ping time to distance in cm 
  Serial.print("  cm = ");   
  Serial.print(cm,DEC);  

  if ((cm < 40) && (cm != 0))
  {
    return(cm);
  }
  else
  {
    return(100);  // No Valid Distance
  }
}// end PingBlink
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
}// PointServo end
/*---------------------------*/


void LookAround()  // Sets next state if Target Found
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
    if (cm_now < TARGET_TOO_CLOSE) digitalWrite(PIN13_LED,HIGH);    

    if (cm_now < 40)
    {
      TargetArray[TARGET_FOUND_ANY ] = true;
    }
    else TargetArray[TARGET_FOUND_ANY ] = false;

    if ((cm_now < TARGET_TOO_CLOSE) && (Direction == 0)) //LEFT
    {
      TargetArray[TARGET_LEFT ] = true;
      Serial.println("TargetLeft");  
    }    

    if ((cm_now < TARGET_TOO_CLOSE) && (Direction == 1)) //Center
    {
      TargetArray[TARGET_CENTER ] = true;
      Serial.println("TargetCenter");       
    }    

    if ((cm_now < TARGET_TOO_CLOSE) && (Direction == 2)) //RIGHT
    {
      TargetArray[TARGET_RIGHT ] = true;
      Serial.println("TargetRight");       
    }    

  }// END Directions
  ForwardSlow();


}// END LookAround

/**************************( STATE MACHINE FUNCTIONS )******************************/
State RoboStartState()
{
  //SpinLeft(2000);
  Serial.println("+++ RoboStartState");
  digitalWrite(PIN13_LED,LOW);    // LED Means Target Too Close
  ForwardFast();  // Start moving forward
  delay(25);
  ForwardSlow();  // Start moving forward
  delay(600);

  RoboGo.Set(RoboStopLookState);    

}// END State RoboStartState


State RoboStopLookState()
{
  Serial.println("+++ RoboStopLookState");

  for (int i = 0; i < TARGET_ARRAY_SIZE; i++) TargetArray[i] = false;
  Stop();  // Start moving forward
  LookAround();     // Ping Ultrasonic in different directions, Set TargetArray

  if      (TargetArray[TARGET_CENTER ] == true)  RoboGo.Set(eboTargetCenter);
  else if (TargetArray[TARGET_LEFT ]   == true)  RoboGo.Set(eboTargetLeft);
  else if (TargetArray[TARGET_RIGHT ]  == true)  RoboGo.Set(eboTargetRight);
  else     toggle2==1;// RoboGo.Set(RoboStartState);  

}// END State RoboStartState

State eboTargetLeft()
{
  Serial.println("***** eboTargetLeft");

  SpinRight(980);
  //delay(1000);
  ForwardFast();
  //delay(1000);
  BackwardFast(255);
  RoboGo.Set(RoboStartState);  
}// END State RoboTargetLeft ----------------------

State eboTargetCenter()  

{
  Serial.println("***** eboTargetCenter");  
  //Stop();
  ForwardFast();
  BackwardFast(255);
  //ForwardFast();
  //ForwardFast();
  //delay(1000);

  // delay(500);

  RoboGo.Set(RoboStartState); 
} // END State RoboTargetCenter -----------------

State eboTargetRight()
{
  Serial.println("***** eboTargetRight");   
  SpinLeft(980);

  ForwardFast();
  BackwardFast(255);
  // delay(1000);
  RoboGo.Set(RoboStartState); 
}// END State RoboTargetRight -----------------

//*********( THE END )***********



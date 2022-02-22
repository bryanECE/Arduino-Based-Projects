//Standard samplpe for v3
// INITIALIZATION:
#define COMPACT_PROXIMITY_SENSOR_LEFT    2       // compact proximity sensor
#define COMPACT_PROXIMITY_SENSOR_CENTER  3       // compact proximity sensor
#define COMPACT_PROXIMITY_SENSOR_RIGHT   4       // compact proximity sensor

#define MOTOR_1_DIRECTION         8       // Motor control HIGH = FWD, LOW = BWD
#define MOTOR_1_SPEED             9       // Speed input
#define MOTOR_2_SPEED            10       // Speed input
#define MOTOR_2_DIRECTION        11       // Motor control HIGH = FWD, LOW = BWD

#define LEFT_LINE_SENSOR          5       // Line sensor left 
#define CENTER_LINE_SENSOR        6       // Line sensor center
#define RIGHT_LINE_SENSOR         7       // Line sensor right

int  COLLISION_SENSE;
int  STUCK_DETECT;
int  LINE_SENSE;
long TIMER=1000;
boolean RAN_DEC;
int  MODE;

// INPUTS AND OUTPUTS
void setup() {                
  Serial.begin(9600);

  // DEFAULT PINS OF THE MOTOR DRIVER (8,9,10,11)
  pinMode(MOTOR_1_DIRECTION, OUTPUT);       
  pinMode(MOTOR_1_SPEED, OUTPUT);
  pinMode(MOTOR_2_SPEED, OUTPUT);
  pinMode(MOTOR_2_DIRECTION, OUTPUT);

  pinMode(LEFT_LINE_SENSOR, INPUT);   
  pinMode(CENTER_LINE_SENSOR, INPUT);
  pinMode(RIGHT_LINE_SENSOR, INPUT);
}

// MAIN PROGRAM
void loop()                     
{
  COLLISION_SENSE=0;
  LINE_SENSE=0;
  if(digitalRead(COMPACT_PROXIMITY_SENSOR_LEFT)   ==LOW) COLLISION_SENSE=1;
  if(digitalRead(COMPACT_PROXIMITY_SENSOR_CENTER) ==LOW) COLLISION_SENSE=COLLISION_SENSE+2;
  if(digitalRead(COMPACT_PROXIMITY_SENSOR_RIGHT)  ==LOW) COLLISION_SENSE=COLLISION_SENSE+4;
  if(digitalRead(LEFT_LINE_SENSOR)  ==LOW) LINE_SENSE=1;
  if(digitalRead(CENTER_LINE_SENSOR)==LOW) LINE_SENSE=LINE_SENSE+2;
  if(digitalRead(RIGHT_LINE_SENSOR) ==LOW) LINE_SENSE=LINE_SENSE+4;

  if(random(1,1000)<500)
    RAN_DEC=LOW;
  else
    RAN_DEC=HIGH;
  if(LINE_SENSE !=0)
  {
    randomSeed(millis()); 
    RUN_BOT(200,200,LOW);
    delay(500);
    ROTATE_BOT(150,RAN_DEC);
    delay(500);
  }
  if(LINE_SENSE ==0)
  {
    if(COLLISION_SENSE==2)
    {
      RUN_BOT(250,250,HIGH);
      delay(250);
    }
    if(COLLISION_SENSE==1)
    {
      analogWrite(MOTOR_2_SPEED, 250);
      delay(100);
    }
    if(COLLISION_SENSE==3)
    {
      analogWrite(MOTOR_2_SPEED, 150);
      delay(100);
    }
    if(COLLISION_SENSE==4)
    {
      analogWrite(MOTOR_1_SPEED, 250);
      delay(100);
    }
    if(COLLISION_SENSE==6)
    {
      analogWrite(MOTOR_1_SPEED, 150);
      delay(100);
    }
  }
  if((LINE_SENSE==0) & (COLLISION_SENSE==0))
  {
    MODE=random(1,7);
    if(TIMER<millis())
    {
      TIMER=millis()+ random(500,1000);
      if(MODE==1) ROTATE_BOT(150,RAN_DEC);
      if(MODE==2) RUN_BOT(150,100,HIGH);
      if(MODE==3) RUN_BOT(100,150,HIGH);
      if(MODE==4) RUN_BOT(100,100,HIGH);
      if(MODE>4) STOP();
    }
  }
}

void RUN_BOT(int spd1,int spd2, boolean direction )
{
  digitalWrite(MOTOR_1_DIRECTION,direction);
  digitalWrite(MOTOR_2_DIRECTION,direction);
  analogWrite(MOTOR_2_SPEED, spd1);
  analogWrite(MOTOR_1_SPEED, spd2); 
}

void ROTATE_BOT(int speed, boolean direction)
{
  digitalWrite(MOTOR_1_DIRECTION,direction);
  digitalWrite(MOTOR_2_DIRECTION,~direction);
  analogWrite(MOTOR_1_SPEED, speed);
  analogWrite(MOTOR_2_SPEED, speed); 
}

void STOP(void)
{
  analogWrite(MOTOR_1_SPEED, 0);
  analogWrite(MOTOR_2_SPEED, 0); 
}
// PROGRAM ENDS AND LOOP BACK FOREVER                                                                                                                                                   // PARZ

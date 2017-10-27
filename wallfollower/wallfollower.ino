// defining motor pins
#define L1 5
#define L2 6
#define R1 11
#define R2 10

// defining ultrasonic sensor pins
#define trigPin1 2
#define echoPin1 3
#define trigPin2 4
#define echoPin2 7
#define trigPin3 8
#define echoPin3 9

// speed parameters
int min_speed = 0;
int max_speed = 255;

// PID parameters
int preverror = 0;
int error = 0;
int derror = 0;
int ierror = 0;

int Kp = 0;
int Kd = 0;
int Ki = 0;

int pd;

// variables to store speed data
int speedL;
int speedR;

//clearence data
const int SIDE_WIDTH = 42; // stage_width - robot_width
const int SIDE_CLEARENCE = 30;
const int FRONT_CLEARENCE = 60;


void setup()
{
  pinMode(L1, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  //Uncomment this if you need to Serial print for debugging  
  //Serial.begin(9600);
}


void loop()
{
  // get sensot data
  long distance1 = readSensor(1);
  long distance2 = readSensor(2);
  long distance3 = readSensor(3);
  
  // a simple technique to eliminat sensor errors.  
  if(distance1>FRONT_CLEARENCE && (distance2+distance3>SIDE_WIDTH))
  {
    if(distance3 > distance2) distance3 = SIDE_WIDTH-distance2; 
    else if(distance2 > distance3) distance2 = SIDE_WIDTH-distance3;  
  }
  
  //controller STRAIGHT
  // If there is wall on both sides of the robot, it should go straight.
  if ( (distance2<SIDE_WIDTH && distance3<SIDE_WIDTH) )
  {
    
    // PID parameter values for the STRAIGHT controller
    Kp = 1;
    Kd = 10;
    
    if ( pd > 0 ) 
    {
      speedR -= pd;
      speedL = max_speed;
    }
    
    
    else if ( pd < 0 ) 
    {
      speedL += pd;
      speedR = max_speed;
    }
  }

  // controller TURN
  // If there is a wall ahead and one of the side is open, it should turn to the open side.
  else if ( (distance1<FRONT_CLEARENCE) && (distance2>SIDE_CLEARENCE ||  distance3>SIDE_CLEARENCE) )
  {    
    // PID parameter values for the TURN controller
    Kp = 6;
    Kd = 8;
    Ki = .7;    
    
    if ( pd > 0 ) 
    {
      speedR -= pd;
      speedL = max_speed;
    }
    else if ( pd < 0 ) 
    {
      speedL += pd;
      speedR = max_speed;
    }
  }
    
  if ( speedR < min_speed )   speedR=min_speed;
  if ( speedL < min_speed )   speedL=min_speed;
  
  if ( speedR > max_speed )   speedR=max_speed;
  if ( speedL > max_speed )   speedL=max_speed;
  

  
  error = distance3 - distance2;
  
  derror = error-preverror;
  ierror+=error;
   
  preverror=error;
  
  pd=Kp*error+Kd*derror+Ki*ierror;  
  
    
    
  analogWrite(L1, speedL);
  analogWrite(L2, 0);
  analogWrite(R1, speedR);
  analogWrite(R2, 0);
    
/*
  // Serial print for error checking
  Serial.print(distance2);
  Serial.print("  ");
  Serial.print(distance1);
  Serial.print("  ");
  Serial.print(distance3);
  Serial.print("        ");
  Serial.print(error);
  Serial.print("        ");
  Serial.print(speedL);
  Serial.print(" ");
  Serial.println(speedR);
*/
  
  
}


long readSensor(int sensor)
{
  int trigPin;
  int echoPin;
  
  if (sensor==1)
  {
    trigPin = trigPin1;
    echoPin = echoPin1;
  }
  else if (sensor==2)
  {
    trigPin = trigPin2;
    echoPin = echoPin2;
  }
  {
    trigPin = trigPin3;
    echoPin = echoPin3;  
  }
  
  
  long duration, distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return (duration/2) / 29.1;
}

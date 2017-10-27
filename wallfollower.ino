#define left_front 5
#define left_back 6
#define right_front 11
#define right_back 10

#define trigPin1 2
#define echoPin1 3
#define trigPin2 4
#define echoPin2 7
#define trigPin3 8
#define echoPin3 9
#define buzz 13

long duration1, distance1;
long duration2, distance2;
long duration3, distance3;

int speedL = 0; 
int speedR = 0;

int speedss = 255;

int preverror = 0;
int error = 0;
int derror = 0;
int ierror = 0;

int Kp = 0;
int Kd = 0;
int Ki = 0;

int pd;

//int timer = -500;



void setup()
{
  pinMode(left_front, OUTPUT);
  pinMode(right_front, OUTPUT);
  pinMode(right_back, OUTPUT);
  pinMode(left_back, OUTPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(buzz, OUTPUT);
  
  Serial.begin(9600);
}


void loop()
{
  Front();
  Right();
  //Left();
  
  

  if ( (distance3>30 || distance2>30) && (distance1>50) )
  {
    if(distance3 > distance2) distance3 = 42-distance2; 
    if(distance2 > distance3) distance2 = 42-distance3;  
  }
  
  
  
  //controller STRAIGHT
  if ( (distance2<40 &&  distance3<40) )
  {
    
    Kp = 1;
    Kd = 10;
    
    speedss = 255;
    
    if ( pd > 0 ) 
    {
      speedR -= pd;
      speedL = speedss;
    }
    
    
    else if ( pd < 0 ) 
    {
      speedL += pd;
      speedR = speedss;
    }
    
    
    
  }
  
  
  
  //controller TURN
  else if ( (distance1<60) && (distance2>30 ||  distance3>30) )
  {
    //timer = millis();
    //speedss=255;
    
    Kp = 6;
    Kd = 8;
    Ki = .7;    
    
    if ( pd > 0 ) 
    {
      speedR -= pd;
      speedL = speedss;
    }
    
    
    else if ( pd < 0 ) 
    {
      speedL += pd;
      speedR = speedss;
    }
  }
  
  
  
  
  
  
  /*
  //controller AfterTurn
  else if ( distance1 > 50 && ( distance2>25 ||  distance3>25 ) )
  {
    
    Kp = 2;
    Kd = 5;
    Ki = .6;
    
    
    //if ( distance1 > 25 ) distance2 = 44-distance1;
    //else if ( distance2 > 25 ) distance1 = 44-distance2;
    
    if ( pd > 0 ) 
    {
      speedR -= pd;
      speedL = speedss;
    }
    
    
    else if ( pd < 0 ) 
    {
      speedL += pd;
      speedR = speedss;
    }
  }
  
  
  
  
  //controller SLANT  
  else 
  {
    
    Kp = 10.5;
    Kd = 10;
    
    if ( pd > 0 ) 
    {
      speedR -= pd;
      speedL = speedss;
    }
    
    
    else if ( pd < 0 ) 
    {
      speedL += pd;
      speedR -= speedss;
    }
  }
  */
    
  
  if ( speedR < 1 )   speedR=0;
  if ( speedL < 1 )   speedL=0;
  
  if ( speedR > speedss )   speedR=speedss;
  if ( speedL > speedss )   speedL=speedss;
  

  
  error = distance3 - distance2;
  
  derror = error-preverror;
  ierror+=error;
   
  preverror=error;
  
  pd=Kp*error+Kd*derror+Ki*ierror;  
  
    
    
  analogWrite(left_front, speedL);
  analogWrite(left_back, 0);
  analogWrite(right_front, speedR);
  analogWrite(right_back, 0);
    

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

  
  
}
void Front()
{
  digitalWrite(trigPin1, LOW);  // Added this line
  delay(2); // Added this line
  digitalWrite(trigPin1, HIGH);
  //  delayMicroseconds(1000); - Removed this line
  delay(10); // Added this line
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1/2) / 29.1;
  
}

void Left()
{
  digitalWrite(trigPin2, LOW);  // Added this line
  delay(2); // Added this line
  digitalWrite(trigPin2, HIGH);
  //  delayMicroseconds(1000); - Removed this line
  delay(10); // Added this line
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2/2) / 29.1;
}

void Right()
{

  
  digitalWrite(trigPin3, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin3, HIGH);
  //  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  distance3 = (duration3/2) / 29.1;
}



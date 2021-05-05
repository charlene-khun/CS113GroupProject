//Line Tracking IO define
#define LT_R !digitalRead(10)
#define LT_M !digitalRead(4)
#define LT_L !digitalRead(2)

//Defining module IO Pin
#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define ENA 6

//Definde car speed
#define carSpeed 150

int counter = 1;
static int n = 0;

//Drive forward
void forward(){
  
  //Write speed to car
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);

  //Write power signals
  digitalWrite(IN1, HIGH); //Enable Left Front motor
  digitalWrite(IN2, LOW); //Disable Left Rear motor
  digitalWrite(IN3, LOW); //Disable Right Rear motor
  digitalWrite(IN4, HIGH); //Enable Right Front motor
  Serial.println("go forward!"); //Output text
}

//Drive back
void back(){
  
  //Write speed to car
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);

  //Write power signals
  digitalWrite(IN1, LOW); //Disable Left Front motor
  digitalWrite(IN2, HIGH); //Enable Left Rear motor
  digitalWrite(IN3, HIGH); //Enable Right Rear motor
  digitalWrite(IN4, LOW); //Disable Right Front motor
  Serial.println("go back!"); //Output text
}

//Turn left
void left(){

  //Write speed to car
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);

  //Write power signals
  digitalWrite(IN1, LOW); //Disable Left Front motor
  digitalWrite(IN2, HIGH); //Enable Left Rear motor
  digitalWrite(IN3, LOW); //Disable Right Rear motor
  digitalWrite(IN4, HIGH); //Enable Right Front motor
  Serial.println("go left!"); //Output text
}

//Turn right
void right(){

  //Write speed to car
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);

  //Write power signals
  digitalWrite(IN1, HIGH); //Enable Left Front motor
  digitalWrite(IN2, LOW); //Disable Left Rear motor
  digitalWrite(IN3, HIGH); //Enable Right Front motor
  digitalWrite(IN4, LOW); //Disable Right Rear motor
  Serial.println("go right!"); //Output text
} 

//Stop
void stop(){
   digitalWrite(ENA, LOW); //No power to the right side
   digitalWrite(ENB, LOW); //No power to the left side
   Serial.println("Stop!"); //Output text
} 

void turn180(int &c1, int &c2, int &c3)
{
  while(c1 != 1)
  {
    right();
    checkSurround(c1,c2,c3);
  }
  
}

void setup(){
  Serial.begin(9600); 
}


void checkSurround(int &c1, int &c2, int &c3)
{
  if(LT_M)
  {
    c1 = 1;
  }
  if(LT_L)
  {
    c2 = 1;
  }
  if(LT_R)
  {
    c3 = 1;
  }

}

bool checkValidity(int c1, int c2, int c3, int TruthTable[8][3])
{
  for(int i = 0; i < 8; i++)
  {
    if(c1 == TruthTable[i][0] && c2 == TruthTable[i][1] && c3 == TruthTable[i][2])
    {
      return true;
    }
  }
  
  return false;
}

void loop() 
{
  bool b;
  int c1 = 0;
  int c2 = 0;
  int c3 = 0;

  int TruthTable[8][3] = {
        {0,0,0},
        {0,0,1},
        {0,1,0},
        {0,1,1},
        {1,0,0},
        {1,0,1},
        {1,1,0},
        {1,1,1},
    };

  do
  {
    checkSurround(c1,c2,c3);
    b = checkValidity(c1,c2,c3, TruthTable);

  } while (b == false);
  
  
  if(c1 == 1 && c2 == 0 && c3 == 0 ) //If the middle sensor is triggered then the car goes straight
  {
    forward(); //Call the forward function
    loop();
  }
  else if(c1 == 0 && c2 == 1 && c3 == 0) //else if the left sensor is triggered the car goes to the left
  {
    left(); //Call the left function while the left sensor is triggered
    loop();
  }
  else if(c1 == 0 && c2 == 0 && c3 == 1) //else if the right sensor is triggered the car goes to the right
  {
    right(); //Call the right function while the right sensor is triggered
    loop();
  }
  else if(c1 == 0 && c2 == 0 && c3 == 0) //If no line in front left or right and counter =1
  {
    if (n == 0)
    {
      turn180(c1,c2,c3);   //turn 180 degrees and loop the program to go back to intial position
      n++;
      loop();  
    }
    else
    {
      stop();
    }
    
  }
}


#include <Servo.h>  //servo library
Servo myservo;      // create servo object to control servo

int Echo = A4;
int Trig = A5;

#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define ENA 6
#define carSpeed 150

/*******************************************************
* Function Name: <forward>
* Purpose: <this function is used to move the car forward>
* Parameter: <no parameter>
* Return: <Void function, no return value>
*******************************************************/
void forward(){
// carSpeed
  analogWrite(ENA, carSpeed); 
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH); //Enable left front motor
  digitalWrite(IN2, LOW); // disable left rear motor
  digitalWrite(IN3, LOW); //disable right rear motor 
  digitalWrite(IN4, HIGH); //enable right front motor 
  Serial.println("Forward"); //output text
}



/*******************************************************
* Function Name: <back>
* Purpose: <This function helps car to move back>
* Parameter: <empty> 
* Return: <Void function, no return value >
*******************************************************/
void back() {
// carSpeed
  analogWrite(ENA, carSpeed); 
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW); //disable left front motor 
  digitalWrite(IN2, HIGH); //enable left rear motor 
  digitalWrite(IN3, HIGH); //enable right rear motor 
  digitalWrite(IN4, LOW); //disable right front motor 
  Serial.println("Back"); //output next
}


/*******************************************************
* Function Name: <left>
* Purpose: < This function helps car to turn left>
* Parameter: <empty >
* Return: <Void function, no return value >
*******************************************************/
void left() {
// carSpeed
  analogWrite(ENA, carSpeed); 
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW); //disable left front motor 
  digitalWrite(IN2, HIGH);  //enable left rear motor 
  digitalWrite(IN3, LOW); //Disable Right Rear motor
  digitalWrite(IN4, HIGH);  //Enable Right Front motor
  Serial.println("Left"); //output text
}


/*******************************************************
* Function Name: <right>
* Purpose: < This function helps car to turn right >
* Parameter: <empty>
* Return: < Void function, no return value>
*******************************************************/
void right() {
// carSpeed
  analogWrite(ENA, carSpeed);  
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);  //Enable Left Front motor
  digitalWrite(IN2, LOW);  //Disable Left Rear motor
  digitalWrite(IN3, HIGH); //Enable Right Front motor
  digitalWrite(IN4, LOW);  //Disable Right Rear motor
  Serial.println("Right"); //output text
} 

/*******************************************************
* Function Name: <stop>
* Purpose: < This function makes the car stop>
* Parameter: <empty>
* Return: < Void function, No return value>
*******************************************************/
void stop() {
  digitalWrite(ENA, LOW); //No power to the right side
  digitalWrite(ENB, LOW); //No power to the left side
  Serial.println("Stop!"); //Output text
}




/*******************************************************
* Function Name: <getDistance>
* Purpose: purpose of the function is to find the distance according to 
            to what angle is the servo motor at that the ultrasonic sensor will
            get the distance at a particular angle and return back the distance
* Parameter: <empty>
* Return: <pluseIN >  //this is the distance that the ultrasonic sensor calculated
*******************************************************/
int getDistance()   
{       
    digitalWrite(Trig, LOW);  //sets the digital pin Trig off
    delayMicroseconds(2);     //waits for 2 microseconds
    digitalWrite(Trig, HIGH); //sets the digital pin Trig on
    delayMicroseconds(10);    //waits for 10 microseconds
    digitalWrite(Trig, LOW);  //sets the digital pin Trig off
    return (int)pulseIn(Echo, HIGH) / 58;
}



/*******************************************************
* Function Name: <setup>
* Purpose: <purpose of the function is to setup>
* Parameter: <empty >
* Return: <Void function, No return value >
*******************************************************/
void setup() 
{
  myservo.attach(3);  // attach servo on pin 3 to servo object
  Serial.begin(9600);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT); //attach pin input
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  stop();
}


/*******************************************************
* Function Name: <checkValidity>
* Purpose: purpose of the function is to check the validity of the values
            c1, c2,c3 and make sure it is in the truth table

* Parameter: <c1, c2, c3 truthtable[8][3] > //passed in values of c1,c2, c3 to check in the 
                                            // two dimensional array

* Return: If c1, c2, c3 matches with any combination in the truth table it will return true
          and if the combination is not in the truth table then the function will return false
*******************************************************/
bool checkValidity(int c1, int c2, int c3, int TruthTable[8][3])
{
  for(int i = 0; i < 8; i++)
  {
    if(c1 == TruthTable[i][0] && c2 == TruthTable[i][1] && c3 == TruthTable[i][2]) //if c1 c2 c3 matches at index i and each positon in the index
    {                                                                              //then return true
      return true;
    }
  }
  
  //if there was no match, then return false
  return false;
}

/*******************************************************
* Function Name: <checksurrounding>
* Purpose: purpose of the function is to calculate c1, c2, c3 values depending upon 
           what is around the car. If the middle distance is less than 30 then c1 = 1
          then the function will change the angle of the servomotor to get the distance of the 
          left distance and the right distance. If the right distance is greater than left distance then
          c2 = 1, c3 = 0 other wise if left distance is greater then c2 = 0, c3 = 1. Last case is if both 
          distances are less than 30 then c2=1, c3=1. 
* Parameter: int &c1, int &c2, int &c3  //These values are passed by reference, so that when c1 c2 c3 gets calculated
                                        //the values will stay changed. This is a good way to return 3 integers.
* Return: < Void function, No return value >
*******************************************************/
void checkSurround(int &c1, int &c2, int &c3)
{
  int middleDistance = 0;
  int rightDistance = 0;
  int leftDistance = 0;
  
  myservo.write(90);  //setservo position according to scaled value
  delay(500);
  middleDistance = getDistance();
  
  if(middleDistance <= 30) //if middledistance is less than or equal to 30 
  {
    //Then c1 gets assigned 1 and then change the angle of the servomotor to get the the distance of the right side at 10 degrees
    c1 = 1;
    
    stop();
    delay(500);
    myservo.write(10);
    delay(1000);
    rightDistance = getDistance();

    //Now we change the angle of the servomotor to 180 degrees to get the distance of the left
    delay(500);
    myservo.write(90);
    delay(1000);
    myservo.write(180);
    delay(1000);
    leftDistance = getDistance();

    //move the servo motor back to 90 degrees
    delay(500);
    myservo.write(90);
    delay(1000);
    
   
    if(rightDistance > leftDistance) //if right distance is greater than left distance then c2 = 1 and c3 = 0 
    {
      c2 = 1;
      c3 = 0; 
    }
    else if(rightDistance < leftDistance) //else if right distance is less than c2 = 0 and c3 = 1 
    { 
      c2 = 0;
      c3 = 1;
    } 
    else if((rightDistance <= 30) && (leftDistance <= 30)) //else if right distance or left distance is less than or equal to 30 then c2 = 1 and c3 = 1
    { 
      c2 = 1;
      c3 = 1;
    } 
   
  }
  else // if forward is greater than 30  
  {
    c1 = 0; 
    c2 = 1;
    c3 = 1;
  }
  
}
/*******************************************************
* Function Name: <loop>
* Purpose: purpose of the function is to loop recursively again and again. Intialize the truth table, use
            a do while loop to get a valid c1, c2, c3 combinataion then performing the action according to 
            c1, c2, c3 values
* Parameter: No parameters
* Return: < Void function, No return value > 
*******************************************************/
//loop to check the surroundings c1, c2 and c3
void loop() 
{
  int c1 = 0;
  int c2 = 0;
  int c3 = 0;

  int TruthTable[8][3] = {
      {0,0,0},
      {0,0,1},
      {0,1,0},
      {0,1,1},  //Forward combination
      {1,0,0},
      {1,0,1}, //Left combination
      {1,1,0}, //Right combination
      {1,1,1}, //Back combination
  };

  //Loop to get a valid c1 c2 c3 combination, keep repeating the loop until the check validity function returns true
  do
  {
    checkSurround(c1,c2,c3);
    b = checkValidity(c1,c2,c3, TruthTable);

  } while (b == false);

  //Series of if and else if statements to perform the action that we associated with a few of the combinations
  //after the action is performed. loop() then gets recursively called again
  if(c1 == 0 && c2 == 1 && c3 == 1) 
  {
    forward(); //Call the forward function, to make the car go forward
    loop();
  }
  else if(c1 == 1 && c2 == 1 && c3 == 0) 
  {
    right();  //Call the right function, to make the car turn right 
    delay(360);
    loop();
  }
  else if(c1 == 1 && c2 == 0 && c3 == 1) 
  { 
    left();   //Call the left function, to make the car turn left
    delay(360);
    loop();
  }
  else if(c1 == 1 && c2 == 1 && c3 == 1) 
  { 
    back();    //Call the back function, to make the car go backwards
    delay(180);
    loop();
  } 
   
}


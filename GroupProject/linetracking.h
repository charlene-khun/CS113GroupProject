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

/*******************************************************
Function Name: <forward>
 Purpose: <purpose of the function is to drive forward>
 Parameter: <empty>
 Return: < Void function, No return value > 
*******************************************************/
void forward()
{  
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

/*******************************************************
Function Name: <back>
 Purpose: <purpose of the function is to drive backwords>
 Parameter: <empty>
 Return: < Void function, No return value > 
*******************************************************/
void back()
{  
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

/*******************************************************
Function Name: <left>
 Purpose: <purpose of the function is to drive to the left>
 Parameter: <empty>
 Return: < Void function, No return value > 
*******************************************************/
void left()
{
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


/*******************************************************
Function Name: <right>
 Purpose: <purpose of the function is to drive to the right>
 Parameter: <empty>
 Return: < Void function, No return value > 
*******************************************************/
void right()
{
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

/*******************************************************
Function Name: <stop>
 Purpose: <purpose of the function is to stop>
 Parameter: <empty>
 Return: < Void function, No return value > 
*******************************************************/
void stop()
{
   digitalWrite(ENA, LOW); //No power to the right side
   digitalWrite(ENB, LOW); //No power to the left side
   Serial.println("Stop!"); //Output text
} 

/*******************************************************
Function Name: <setup>
 Purpose: <purpose of the function is to setup the line tracking>
 Parameter: <empty>
 Return: < Void function, No return value > 
*******************************************************/
void setup(){
  Serial.begin(9600); 
}

/*******************************************************
Function Name: <turn180>
 Purpose: <purpose of the function is to make the car turn 180 degrees>
 Parameter: int &c1, int &c2, int &c3  //These vales are passed by reference, so that when c1 gets 1 then the loop will stop
 Return: < Void function, No return value >  
*******************************************************/
void turn180(int &c1, int &c2, int &c3)
{
  while(c1 != 1)
  {
    right();
    checkSurround(c1,c2,c3);
  }
  
}


//checks surroundings on the line
/*******************************************************
Function Name: <checkSurround>
 Purpose: <purpose of the function is to check where is the line being detected>
 Parameter: int &c1, int &c2, int &c3  //These values are passed by reference, so that when c1 c2 c3 gets calculated
                                        //the values will stay changed. This is a good way to return 3 integers.
 Return: < Void function, No return value >  
*******************************************************/
void checkSurround(int &c1, int &c2, int &c3)
{
  //initially c1 c2 c3 are set to 0 from the loop function

  if(LT_M) //if the line is in the middle then set c1 = 1
  {
    c1 = 1;
  }
  if(LT_L) //if the line is in the left then set c2 = 1
  {
    c2 = 1;
  }
  if(LT_R) //if the line is in the right then set c3 = 1
  {
    c3 = 1;
  }

}


/*******************************************************
Function Name: <checkValidity>
 Purpose: <purpose of the function is to check to see if the combination of c1 c2 c3 values that gets passed into the function 
           is in the truth table>
 Parameter: int c1, int c2, int c3, int TruthTable[8][3] //passed in values of c1,c2, c3 to check in the 
                                                          // two dimensional array
 Return: If c1, c2, c3 matches with any combination in the truth table it will return true
          and if the combination is not in the truth table then the function will return false
*******************************************************/
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
/*******************************************************
Function Name: <loop>
 Purpose:   purpose of the function is to loop recursively again and again. Intialize the truth table, use
            a do while loop to get a valid c1, c2, c3 combinataion then performing the action according to 
            c1, c2, c3 values
 Parameter: <none>
 Return: < Void function, No return value >
*******************************************************/
void loop() 
{
  bool b;
  int c1 = 0;
  int c2 = 0;
  int c3 = 0;

  int TruthTable[8][3] = {
        {0,0,0}, //turn180 combination
        {0,0,1}, //Right combination
        {0,1,0}, //Left combiantion
        {0,1,1},
        {1,0,0}, //Forward combination
        {1,0,1},
        {1,1,0},
        {1,1,1},
    };

  //Loop to get a valid c1 c2 c3 combination, keep repeating the loop until the check validity function returns true
  do
  {
    checkSurround(c1,c2,c3);
    b = checkValidity(c1,c2,c3, TruthTable);

  } while (b == false);
  
  //Series of if and else if statements to perform the action that we associated with a few of the combinations
  //after the action is performed. loop() then gets recursively called again
  if(c1 == 1 && c2 == 0 && c3 == 0 ) 
  {
    forward(); //Call the forward function, to make the car go forward
    loop();
  }
  else if(c1 == 0 && c2 == 1 && c3 == 0) 
  {
    left(); //Call the left function, to make the car go left
    loop();
  }
  else if(c1 == 0 && c2 == 0 && c3 == 1) 
  {
    right(); //Call the right function, to make the car go right
    loop();
  }
  else if(c1 == 0 && c2 == 0 && c3 == 0) 
  {
    turn180(c1,c2,c3); //Call the turn180 function, to make the car turn 180 degrees
    loop();
  }
}

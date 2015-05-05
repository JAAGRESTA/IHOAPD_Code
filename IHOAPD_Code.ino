/* IHOAPD - Infrared Head-Operated Assistive Pointing Device Arduino Code
*  ECE 2799 Team 8: Joe Agresta, Giselle Verbera, and Sean Watson
*  Author - Joe Agresta
*  5/5/2015
*/

//sensor pin configuration, analog pins
#define leftSensorPin A0 
#define rightSensorPin A1 
#define topSensorPin A2
#define downSensorPin A3

//buttons, digital pins
#define leftButtonPin 5
#define rightButtonPin 2

//these were for possibly implementing LEDs that would light up to 
//indicate which sensor was being looked at by the user, not implemented
//#define leftLEDPin 9 
//#define rightLEDPin 10
//#define topLEDPin 11
//#define downLEDPin 12

//cursor speed constants
#define slowSpd 1700
#define fastSpd 500

int topVal = 0, downVal = 0, leftVal = 0, rightVal = 0; //holders for sensor values
int threshold = 200; //threshold that below which indicates that a sensor is definitely being looked at
int hCal = 0, vCal = 0; //calibration variables, not used in current implementation
int speed_flag = 0; 
int countSpd = fastSpd; //set inital cursor speed to fast
int cursorStep = 1; //smallest mouse movement step possible 

//one-time code
void setup()
{
  Serial.begin(9600);
  pinMode(leftSensorPin, INPUT);
  pinMode(rightSensorPin, INPUT);
  pinMode(topSensorPin, INPUT);
  pinMode(downSensorPin, INPUT);
  pinMode(leftButtonPin, INPUT);
  //pinMode(rightButtonPin, INPUT); //right click mode, not an interrupt
  attachInterrupt(1, toggleSpeedISR, RISING); //sensitivity control using right click
  Mouse.begin();
}

//main code
void loop()
{
    checkButtons(); //check for clicks
    readSensors(); //get sensor values
    compareSensors(countSpd); //movemouse based on inputs 
}

//reads all IR sensor values and sets the corresponding variables
void readSensors()
{
	getLeftVal();
	getRightVal();
	getTopVal();
	getDownVal();
}

//changes sensitivity if right click is pressed
void toggleSpeedISR()
{
  if(speed_flag == 0)
  {
    speed_flag = 1;
    countSpd = slowSpd; //slow mouse cursor movement
  }
  else if(speed_flag == 1)
  {
    speed_flag = 0; 
    countSpd = fastSpd; //speed up mouse cursor movement
  }  
}

//looks at button voltage values, decides if any clicks should happen or not
void checkButtons()
{
  checkLeftButton();
  checkRightButton();
}

//issues a right click command if the right button is pressed
void checkRightButton()
{
    //right click, replaced with speed sensitivity function
//   if(digitalRead(rightButtonPin) == HIGH) //right button is active high for some reason
//  {
//     Mouse.press(MOUSE_RIGHT); //start right click
//  }
//  else if(digitalRead(rightButtonPin) == LOW)
//  {
//     Mouse.release(MOUSE_RIGHT); //end right click
//  }
}

//sends command to do a left click if the button is being pushed
//button can be held the same was as a normal mouse
void checkLeftButton()
{
  //left click
  if(digitalRead(leftButtonPin) == LOW) //left button is active low
  {
    Mouse.press(MOUSE_LEFT); //start left click
  }
  else if(digitalRead(leftButtonPin) == HIGH)
  {
    Mouse.release(MOUSE_LEFT); //end left click
  }
}

/*interprets sensors values and moves mouse accordingly.
 *parameter: count - int value representing how many iterations should occur 
 *before one movement, increasing count makes the cursor move slower
 */
void compareSensors(int count)
{ 
  int i = 0;
  int exit = 0;
  while(exit != 1)
  {
    //mouse movements only occur when i equals the count value causing movements to only happen once per a number of iterations equal to count
    if((topVal < threshold) && (topVal < downVal) && (i == count)) //checks if the top sensor is being illuminated, compares to bottom
    {
      Mouse.move(0, cursorStep, 0); //move mouse up
      //digitalWrite(topLEDPin, HIGH); //user feeback LEDs, not implemented
      //digitalWrite(downLEDPin, LOW);
    }
    else if ((downVal < threshold) && (downVal < topVal) && (i == count)) //checks if the bottom sensor is being illuminated, compares to top
    {
      Mouse.move(0, -1*cursorStep, 0); //move mouse down
      //digitalWrite(downLEDPin, HIGH);
      //digitalWrite(topLEDPin, LOW);
    } 
    if ((leftVal < threshold) && (leftVal < rightVal) && (i == count)) //checks if the left sensor is being illuminated, compares to right
    {
      Mouse.move(-1*cursorStep, 0, 0); //move mouse left
      //digitalWrite(leftLEDPin, HIGH);
      //digitalWrite(rightLEDPin, LOW);
    } 
    else if((rightVal < threshold) && (rightVal < leftVal) && (i == count)) //checks if the right sensor is being illuminated, compares to left
    {
      Mouse.move(cursorStep, 0, 0); //move mouse right
      //digitalWrite(rightLEDPin, HIGH);
      //digitalWrite(leftLEDPin, LOW);
    }
    if(i == count)
    {
      exit = 1;
    }
    i++; 
  }
}

//reads the value of the left IR sensor 
void getLeftVal()
{
	leftVal = analogRead(leftSensorPin);
}

//reads the value of the right IR sensor 
void getRightVal()
{
	rightVal = analogRead(rightSensorPin);
}

//reads the value of the top IR sensor 
void getTopVal()
{
	topVal = analogRead(topSensorPin);
}

//reads the value of the bottom IR sensor 
void getDownVal()
{
	downVal = analogRead(downSensorPin);
}

//prints the sensor values in the serial monitor for testing
void printValues()
{
 Serial.print("Left = "); 
 Serial.print(leftVal); 
 Serial.print(" , Right = ");
 Serial.print(rightVal);
 Serial.print(" , Top = ");
 Serial.print(topVal);
 Serial.print(" , Bottom = ");
 Serial.println(downVal);
}

//get values from each axis and subtracts them to set calbration values, not implemented in current sensor configuration
void calibrate()
{
  getLeftVal();
  getRightVal();
  getTopVal();
  getDownVal();
  hCal = leftVal - rightVal;
  vCal = topVal - downVal;
}

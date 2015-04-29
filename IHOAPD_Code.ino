/* IHOAPD - Infrared Head-Operated Assistive Pointing Device Arduino Code
*  ECE 2799 Team 8: Joe Agresta, Giselle Verbera, and Sean Watson
* 
*/
//pin configuration
#define leftSensorPin A0
#define rightSensorPin A1
#define topSensorPin A2
#define downSensorPin A3
#define leftButtonPin 3
#define rightButtonPin 2
#define leftLEDPin 4
#define rightLEDPin 5
#define topLEDPin 6
#define downLEDPin 7
#include "TimerOne.h"
 

int topVal = 0, downVal = 0, leftVal = 0, rightVal = 0;
int threshold = 200;
int hCal = 0, vCal = 0;
int timer_flag = 0;
float cursorStep = 1;
float sampleTimeMs = 35;
float timerPer;
float flopDelay1 = 2000;
float flopDelay2 = 2000;

//one-time code
void setup()
{
  timerPer = sampleTimeMs * 1000;
  
  Serial.begin(9600);
  pinMode(leftSensorPin, INPUT);
  pinMode(rightSensorPin, INPUT);
  pinMode(topSensorPin, INPUT);
  pinMode(downSensorPin, INPUT);
  pinMode(leftButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);
  Timer1.initialize(timerPer);
  Timer1.attachInterrupt(flagISR);
  Mouse.begin();
}

//main code
void loop()
{
  if(timer_flag == 1)
  {
    checkButtons();
    readSensors();
    compareSensors();
   // flopDelay(15);
  }
  
}

//reads all IR sensor values and sets the corresponding variables
void readSensors()
{
	getLeftVal();
	getRightVal();
	getTopVal();
	getDownVal();
}

//get values from each axis and subtract them to set calbration values, not implemented in current sensor configuration
void calibrate()
{
  getLeftVal();
  getRightVal();
  getTopVal();
  getDownVal();
  hCal = leftVal - rightVal;
  vCal = topVal - downVal;
}

//everytime the timer triggers an interrupt change flag
void flagISR()
{
  if(timer_flag == 0)
  {
     timer_flag = 1; 
  }
  else if(timer_flag == 1)
  {
     timer_flag = 0;  
  }
}

//wast time
void flopDelay(int n)
{
 int i = 0;
 while(i < n)
 {
   flopDelay1 = flopDelay1 + flopDelay2;
   i++;
   flopDelay1 = 2000;
 } 
}
//test code for reading a single IR sensor
void IRtest()
{
  getLeftVal();
  if(leftVal > threshold)
  {
   Mouse.move(-1*cursorStep, 0, 0); //move mouse left
  }
  else 
  {
    //Mouse.move(cursorStep, 0, 0); //move mouse right
  }
  Serial.println(leftVal);
}

//looks at button voltage values, decides if a click should happen or not
void checkButtons()
{
  if(digitalRead(leftButtonPin) == LOW) //left button is active low
  {
    Mouse.press(MOUSE_LEFT); //start left click
  }
  else if(digitalRead(leftButtonPin) == HIGH)
  {
    Mouse.release(MOUSE_LEFT); //end left click
  }
   if(digitalRead(rightButtonPin) == HIGH) //right button is active high for some reason
  {
    Mouse.press(MOUSE_RIGHT); //start right click
  }
  else if(digitalRead(rightButtonPin) == LOW)
  {
    Mouse.release(MOUSE_RIGHT); //end right click
  }
}

//interprets sensors values and moves mouse accordingly, issues user feedback
void compareSensors()
{ 
  if((topVal < threshold) && (topVal < downVal)) //checks if the top sensor is being illuminated more than the bottom sensor
  {
    Mouse.move(0, cursorStep, 0); //move mouse up
    //digitalWrite(topLEDPin, HIGH); //user feeback LEDs
    //digitalWrite(downLEDPin, LOW);
  }
  else if ((downVal < threshold) && (downVal < topVal)) //checks if the bottom sensor is being illuminated more than the top sensor
  {
    Mouse.move(0, -1*cursorStep, 0); //move mouse down
    //digitalWrite(downLEDPin, HIGH);
    //digitalWrite(topLEDPin, LOW);
  } 
  if ((leftVal < threshold) && (leftVal < rightVal)) //checks if the left sensor is being illuminated more than the right sensor
  {
    Mouse.move(-1*cursorStep, 0, 0); //move mouse left
    //digitalWrite(leftLEDPin, HIGH);
    //digitalWrite(rightLEDPin, LOW);
  } 
  else if((rightVal < threshold) && (rightVal < leftVal)) //checks if the right sensor is being illuminated more than the left sensor
  {
    Mouse.move(cursorStep, 0, 0); //move mouse right
    //digitalWrite(rightLEDPin, HIGH);
    //digitalWrite(leftLEDPin, LOW);
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

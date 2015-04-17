/* IHOAPD - Infrared Head-Operated Assistive Pointing Device Arduino Code
*  ECE 2799 Team 8: Joe Agresta, Giselle Verbera, and Sean Watson
* 
*/

#include "TimerOne.h"
//pin configuration
#define leftSensorPin A0
#define rightSensorPin A1
#define topSensorPin A2
#define downSensorPin A3
#define leftButtonPin 3
#define rightButtonPin 2
#define ledPin 10
int topVal = 0, downVal = 0, leftVal = 0, rightVal = 0;
int hThreshold = 600;
int lThreshold = 300;
int lightThreshold = 915;
int timerFlag = 0;
int burstCounter = 0;
int hCal = 0, vCal = 0;
float cursorStep = 1;

//button states
enum clickState 
 {
   clicked,
   released
 };
clickState left = released;
clickState right = released;

//one-time code
void setup()
{
  Serial.begin(9600);
  pinMode(leftSensorPin, INPUT);
  pinMode(rightSensorPin, INPUT);
  pinMode(topSensorPin, INPUT);
  pinMode(downSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Timer1.initialize(52);
  Timer1.attachInterrupt(pulseLED);
  attachInterrupt(0, leftClickISR, RISING); //interrupt 0 is pin 3 on the leonardo
  // attachInterrupt(1, rightClickISR, CHANGE); //interrupt 1 is pin 2 on the leonardo
  Mouse.begin();
}

//main code
void loop()
{
  IRtest();
  //readSensors();
  //printValues();
  //compareSensors(); 
  // hTest();
    
}

//reads all IR sensor values and sets the corresponding variables
void readSensors()
{
	getLeftVal();
	getRightVal();
	getTopVal();
	getDownVal();
}

//get values from each axis and subtract them to set calbration values
void calibrate()
{
  getLeftVal();
  getRightVal();
  getTopVal();
  getDownVal();
  hCal = leftVal - rightVal;
  vCal = topVal - downVal;
}

//ISR to pulse LED based off of Timer1 interrupt
void pulseLED()
{
 if(timerFlag == 0 && ((burstCounter <= 10) != 0))
 {
   digitalWrite(ledPin, HIGH);
   timerFlag = 1;
 }
 else if(timerFlag == 1)
 {
   digitalWrite(ledPin, LOW);
   timerFlag =0 ;
 } 
 if(burstCounter == 20)
 {
   burstCounter = 0;
 }
  burstCounter++;
}
//etch-a-sketch test
void easTest()
{
   if(leftVal > hThreshold)
  {
    Mouse.move(cursorStep, 0, 0); //move mouse right
  }
  else if (leftVal < lThreshold)
  {
    Mouse.move(-1*cursorStep, 0, 0); //move mouse left
  }
  
   if(rightVal > hThreshold)
  {
      Mouse.move(0, -1*cursorStep, 0); //move mouse down
  }
  else if (rightVal < lThreshold)
  {
      Mouse.move(0, cursorStep, 0); //move mouse up
  }
}

//horizontal cursor test using photo-resistors
void hTest()
{
  if ((leftVal > lightThreshold) && (leftVal - hCal > rightVal))
  {
    Mouse.move(-1*cursorStep, 0, 0); //move mouse left
  } 
  else if((rightVal > lightThreshold) && (rightVal > leftVal - hCal))
  {
    Mouse.move(cursorStep, 0, 0); //move mouse right
  }
   if(topVal > hThreshold)
  {
    Mouse.move(0, -1*cursorStep, 0); //move mouse down
  }
  else if (topVal < lThreshold)
  {
    Mouse.move(0, cursorStep, 0); //move mouse up 
  }
}

//test code for reading a single IR sensor
void IRtest()
{
  getLeftVal();
  Serial.println(leftVal);
}


void compareSensors()
{ 
//	if((topVal > threshold) && (topVal > downVal))
//	{
//		Mouse.move(0, cursorStep, 0); //move mouse up
//	}
//	else if ((downVal > threshold) && (downVal > topVal))
//	{
//		Mouse.move(0, -1*cursorStep, 0); //move mouse down
//	} 
//	if ((leftVal > threshold) && (leftVal > rightVal))
//	{
//		Mouse.move(-1*cursorStep, 0, 0); //move mouse left
//	} 
//	else if((rightVal > threshold) && (rightVal > leftVal))
//	{
//		Mouse.move(cursorStep, 0, 0); //move mouse right
//	}
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

//prints left and right sensor values in serial monitor
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
//hold left button when pressed until it is released, interrupt service routine
void leftClickISR() 
{
	if(left == released)
	{
		Mouse.press(MOUSE_LEFT);
		left = clicked;    
	}
	else if(left == clicked)
	{
		Mouse.release(MOUSE_LEFT);
		left = released;
	}
}

//hold right button when pressed until it is released, interrupt service routine
void rightClickISR() 
{
	if(right == released)
	{
		Mouse.press(MOUSE_RIGHT);
		right = clicked;    
	}
	else if(right == clicked)
	{
		Mouse.release(MOUSE_RIGHT);
		right = released;
	}
}

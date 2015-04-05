/* IHOAP - Infrared Head-Operated Assistive Pointing Device Arduino Code
*  ECE 2799 Team 8: Joe Agresta, Giselle Verbera, and Sean Watson
* 
*/
#include <Mouse.h>
#define leftSensorPin A0
#define rightSensorPin A1
#define topSensorPin A2
#define downSensorPin A3
#define leftButtonPin 3
#define rightButtonPin 2
int topVal = 0, downVal = 0, leftVal = 0, rightVal = 0;
int threshold = 300;
int cursorStep = 10;

enum clickState 
{
	clicked;
	released;
}

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
  Mouse.begin();
  attachInterrupt(0, leftClickISR, CHANGE); //interrupt 0 is pin 3 on the leonardo
  attachInterrupt(1, rightClickISR, CHANGE); //interrupt 1 is pin 2 on the leonardo

}

//main code
void loop()
{
	readSensors();
	compareSensors(); 
}

//reads all IR sensor values and sets the corresponding variables
void readSensors()
{
	getLeftVal();
	getRightVal();
	getTopVal();
	getDownVal();
}

void compareSensors()
{
	if((topVal > threshold) && (topVal > downVal))
	{
		Mouse.move(0, cursorStep, 0); //move mouse up
	}
	else if ((downVal > threshold) && (downVal > topVal))
	{
		Mouse.move(0, -1*cursorStep, 0); //move mouse down
	} 
	if ((leftVal > threshold) && (leftVal > rightVal))
	{
		Mouse.move(-1*cursorStep, 0, 0); //move mouse left
	} 
	else if((rightVal > threshold) && (rightVal > leftVal))
	{
		Mouse.move(cursorStep, 0, 0); //move mouse right
	}
}


//reads the value of the left sensor (an int from 0-1024)
void getLeftVal()
{
	leftVal = analogRead(leftSensorPin);
}

//reads the value of the right sensor 
void getRightVal()
{
	rightVal = analogRead(rightSensorPin);
}

//reads the value of the top sensor 
void getTopVal()
{
	topVal = analogRead(topSensorPin);
}

//reads the value of the bottom sensor 
void getDownVal()
{
	downVal = analogRead(downSensorPin);
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
		Mouse.released(MOUSE_LEFT);
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
		Mouse.released(MOUSE_RIGHT);
		right = released;
	}
}
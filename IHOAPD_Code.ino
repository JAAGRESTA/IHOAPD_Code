/* IHOAPD - Infrared Head-Operated Assistive Pointing Device Arduino Code
*  ECE 2799 Team 8: Joe Agresta, Giselle Verbera, and Sean Watson
* 
*/

#define leftSensorPin A0
#define rightSensorPin A1
#define topSensorPin A2
#define downSensorPin A3
#define leftButtonPin 3
#define rightButtonPin 2
int topVal = 0, downVal = 0, leftVal = 0, rightVal = 0;
int hThreshold = 600;
int lThreshold = 300;
int lightThreshold = 900;
int hCal = 0, vCal = 0;
float cursorStep = 1;

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
  Mouse.begin();
  attachInterrupt(0, leftClickISR, RISING); //interrupt 0 is pin 3 on the leonardo
 // attachInterrupt(1, rightClickISR, CHANGE); //interrupt 1 is pin 2 on the leonardo

}

//main code
void loop()
{
  readSensors();
  printValues();
	//compareSensors(); 
  hTest();
        delay(3);
}

//reads all IR sensor values and sets the corresponding variables
void readSensors()
{
	getLeftVal();
	getRightVal();
	getTopVal();
	getDownVal();
}

void calibrate()
{
  getLeftVal();
  getRightVal();
  getTopVal();
  getDownVal();
  hCal = leftVal - rightVal;
  vCal = topVal - downVal;
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

//prints left and right sensor values in serial monitor
void printValues()
{
 Serial.print("(L,R,T,B) = "); 
 Serial.print(leftVal); 
 Serial.print(",");
 Serial.print(rightVal);
 Serial.print(",");
 Serial.print(topVal);
 Serial.print(",");
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

////hold right button when pressed until it is released, interrupt service routine
//void rightClickISR() 
//{
//	if(right == released)
//	{
//		Mouse.press(MOUSE_RIGHT);
//		right = clicked;    
//	}
//	else if(right == clicked)
//	{
//		Mouse.release(MOUSE_RIGHT);
//		right = released;
//	}
//}

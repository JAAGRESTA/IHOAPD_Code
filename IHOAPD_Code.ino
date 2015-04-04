/* IHOAP - Infrared Head-Operated Assistive Pointing Device Arduino Code
*  ECE 2799 Team 8: Joe Agresta, Giselle Verbera, and Sean Watson
* 
*/

#define leftControlPin A0
#define rightControlPin A1
int xPos = 0, yPos = 0;


//one-time code
void setup()
{
  serial.begin(9600);
  pinMode(leftControlPin, INPUT);
  pinMode(rightControlPin, INPUT);

}

//main code
void loop()
{
  xposPos = getLeftVal();
  y = getRightVal();
  serial.println("("+ x + "," + y + ")")
}

//reads left potentiometer value for testing
void getLeftVal()
{
	analogRead(leftControlPin);
}

//reads right potentiometer value for testing
void getRightVal()
{
	analogRead(rightControlPin);
}
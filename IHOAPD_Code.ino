/* IHOAP - Infrared Head-Operated Assistive Pointing Device Arduino Code
*  ECE 2799 Team 8: Joe Agresta, Giselle Verbera, and Sean Watson
* 
*/
#include Mouse
#define leftControlPin A0
#define rightControlPin A1
int xPos = 0, yPos = 0;


//one-time code
void setup()
{
  Serial.begin(9600);
  pinMode(leftControlPin, INPUT);
  pinMode(rightControlPin, INPUT);

}

//main code
void loop()
{
 	  getLeftVal();
 	  getRightVal();
 	  Serial.print("(");
 	  Serial.print(xPos);
 	  Serial.print(",");
  	  Serial.print(yPos);
  	  Serial.println(")");
}

//reads left potentiometer value for testing
void getLeftVal()
{
	xPos = analogRead(leftControlPin);
}

//reads right potentiometer value for testing
void getRightVal()
{
	yPos = analogRead(rightControlPin);
}
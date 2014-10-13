//Libraries
#include <Wire.h>

#include "Motor_commands.h"
#include "Sensor_fnctions.h"

#define startbyte 0x0F
#define I2Caddress 0x07

//Definitions
#ECHO_TO_SERIAL 1


//Sensor variables
//int pinI1 = 8;//define I1 interface
//int pinI2 = 11;//define I2 interface
//int speedpinA = 9;//enable motor A
//int pinI3 = 12;//define I3 interface
//int pinI4 = 13;//define I4 interface
//int steerpinB = 10;//enable motor B
//int speed_drive = 420;//define the speed of drive motor
//int speed_steer = 1524;//define the speed of steering motor
const int pingPin_R = 22;//sets the pin for the PING sensor on the right side of the car
const int echoPin_R = 23;//sets the pin for the ECHO sensor on the right side of the car
const int pingPin_C = 24;//sets the pin for the PING sensor in the center side of the car
const int echoPin_C = 25;//sets the pin for the ECHO sensor on the center side of the car
const int pingPin_L = 26;//sets the pin for the PING sensor on the left side of the car
const int echoPin_L = 27;//sets the pin for the ECHO sensor on the left side of the car
long duration_C, cm_C, duration_R, cm_R, duration_L, cm_L;

void setup()
{
  #if ECHO_TO_SERIAL
  Serial.begin(115200);
  #endif
  Wire.begin();
}

void loop()
{ 
  int i;
  int j = 1;
  delay (2000); //Can be increased to 5000 if timing is off
  for(i=0; i<1; i*1)
  {
    j++;
    pingAll();
    
    
    //all clear, no obstacles within 250cm in any direction
  if(cm_C >= 250 && cm_R >= 250 && cm_L >= 250)
  {
   forward();
  }
 
//obstacle(s) within 0-30cm range
  else if (cm_L < 30 || cm_C < 30 || cm_R < 30)
  {
   stop();
   backward();
   delay(1500);
   stop();
     if(j%3 == 0){ //This if/else statement is designed to build a little "randomness"
     back_right(); //into the robot's movements, so it is less likely to become stuck
     back_right(); //in a loop, performing the same actions over and over which only.
     left();
     j=1;
     }
   else{
     back_left();
     back_left();
     right();
     }
  }
 
//obstacle(s) within 30cm-60cm range
 
  //obstacle on left and center and right
  else if (cm_R < 60 && cm_C < 60 && cm_L < 60)
  {
   stop();
   backward();
   delay(1500);
   back_left();
   stop();
   right();
   forward();
  }
  //obstacle on center OR left and right
  else if (cm_L >= 60 && cm_R >= 60 && cm_C < 60 ||
            cm_C >= 60 && cm_R < 60 && cm_L < 60)
  {
   stop();
   backward();
   delay(1500);
   back_left();
   stop();
   right();
   forward();
  }
  //obstacle on left and center
  else if (cm_R >= 60 && cm_C < 60 && cm_L < 60)
  {
   stop();
   backward();
   delay(1500);
   back_left();
  }
  //obstacle on right and center
  else if (cm_L >= 60 && cm_C < 60 && cm_R < 60)
  {
   stop();
   backward();
   delay(1500);
   back_right();
  }
  //obstacle on right
  else if (cm_L >= 60 && cm_C >= 60 && cm_R < 60)
  {
   left();
   left();
   
  }
  //obstacle on left
  else if (cm_R >= 60 && cm_C >= 60 && cm_L < 60)
  {
   right();
   right();
  }
 
//obstacle(s) within 60cm-120cm range
 
  //obstacle on left and center
  else if (cm_R >= 120 && cm_C < 120 && cm_L < 120)
  {
   right();
   right();
  }
  //obstacle on right and center
  else if (cm_L >= 120 && cm_C < 120 && cm_R < 120)
  {
   left();
   left();
  }
  //obstacle on right and left
  else if (cm_C >= 120 && cm_L < 120 && cm_R < 120)
  {
   forward();
  }
  //obstacle on right
  else if (cm_L >= 120 && cm_C >= 120 && cm_R < 120)
  {
   left();
  }
  //obstacle on left
  else if (cm_R >= 120 && cm_C >= 120 && cm_L < 120)
  {
   right();
  }
  //obstacle on center
  else if (cm_L >= 120 && cm_R >= 120 && cm_C < 120)
  {
   if(j % 2 == 0){
     left();
     j=1;
   }
   else{
     right();
    }
  }  
  }
}

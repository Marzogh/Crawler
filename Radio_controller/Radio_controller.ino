
 // ==========================================================================================================================================================================================================================================================================  //
 //                                                                                                                                                                                                                                                                             //
 //                                                             |''||''| '||               .|'''',                           '||`                  '||'''|,                                    ||                                                                               //
 //                                                                ||     ||               ||                                 ||                    ||   ||                  ''                ||                                                                               //
 //                                                                ||     ||''|, .|''|,    ||      '||''|  '''|.  '\\    //`  ||  .|''|, '||''|     ||...|' '||''| .|''|,    || .|''|, .|'', ''||''                                                                             //
 //                                                                ||     ||  || ||..||    ||       ||    .|''||    \\/\//    ||  ||..||  ||        ||       ||    ||  ||    || ||..|| ||      ||                                                                               //
 //                                                               .||.   .||  || `|...     `|....' .||.   `|..||.    \/\/    .||. `|...  .||.      .||      .||.   `|..|'    || `|...  `|..'   `|..'                                                                            //
 //                                                                                                                                                                                                                                                                             //
 //                                                                              An autonomous rover designed and built around a 1/5th Rock Crawler chasis by Prajwal Bhattaram                                                                                                 //
 //                                                                                                                   v.0.1.0-beta                                                                                                                                              //
 //                                                                                                                    21.Oct.2014                                                                                                                                              //
 //                                                                                                   https://github.com/Marzogh/The-Crawler-Project                                                                                                                            //
 //                                                                                                                                                                                                                                                                             //
 //                                                                                                                   The Radio Controller                                                                                                                                      //
 //                                                                                                                                                                                                                                                                             // 
 // ==========================================================================================================================================================================================================================================================================  // 

#define up_button	8                                         // Button power
#define down_button	A1                                        // Button go
#define left_button	9                                         // Button RTH
#define right_button	12                                        // Button RC

#define stick_button	A0                                        // Button stick
#define level_stick	A2                                        // Steering
#define vertical_stick	A3                                        // Speed
#define DEBOUNCE 10                                               // Set debounce time (10ms in this case)

HardwareSerial radio = Serial;                                    // Connect radio to Serial port

int IO, mode, spd, brk, dir, res = 0;                             // Variables sent to crawler
int RTH, RC, power, Auto, setHome, steering, speed;               // Variables on controller
int previousRTH, previousRC, previousPower,
    previousAuto, previousSetHome = HIGH;    // the previous reading from the input pin
long prevtime, time = 0;

void setup () 
{
  radio.begin (115200);
  
  pinMode (left_button, INPUT);
  pinMode (right_button, INPUT);
  pinMode (up_button, INPUT);
  pinMode (down_button, INPUT);
  
  pinMode (stick_button  , INPUT);
  pinMode (level_stick   , INPUT);
  pinMode (vertical_stick, INPUT);
  
  readInputs();
  powerCheck();
  modeCheck();
  transmitCommand();
  
  #if DEBUG
  Serial.print (powerOff);
  Serial.print (" ");
  Serial.print (powerOn);
  Serial.print (" ");
  Serial.print (RTH);
  Serial.print (" ");
  Serial.print (RC);
  Serial.print (" ");
  Serial.print (speed);
  Serial.print (" ");
  Serial.print (steering);
  Serial.print (" ");
  Serial.println (setHome);
  #endif
}


void loop () 
{
  while (mode == 2)
  {
    spd = map(speed, 0, 1018, 255, -255);                              // Map values from joystick to speed (The joysitck on my input shield centers (vertical, horizontal) at (509, 530). The values are mapped to match)
    spd = constrain(spd, 255, -255);                                   // Constrain values
    
    if (speed == 509)                                                  // If joystick is centered
    brk = 255;                                                         // Apply brakes
    
    dir = map(steering, 1060, 0, 1000, 2000);                          // Map values from joysitck to turning angle
    dir = constrain(dir, 1000, 2000);                                  // Constrain values
    
    if (prevtime+10 < millis())
    {
      transmitCommand();
      prevtime = millis();
    }
    transmitCommand();
    readInputs();
    powerCheck();
    modeCheck();
  }
  
  while (mode == 1 || mode == 2)
  {
    readInputs();
    powerCheck();
    modeCheck();
    
    if (prevtime+10 < millis())
    {
      transmitCommand();
      prevtime = millis();
    }
  }
  

}




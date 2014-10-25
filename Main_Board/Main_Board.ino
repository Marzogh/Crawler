  
 // =====================================================================================================================================  //
 //                                                                                                                                        //
 // |''||''| '||               .|'''',                           '||`                  '||'''|,                                    ||      //
 //    ||     ||               ||                                 ||                    ||   ||                  ''                ||      //
 //    ||     ||''|, .|''|,    ||      '||''|  '''|.  '\\    //`  ||  .|''|, '||''|     ||...|' '||''| .|''|,    || .|''|, .|'', ''||''    //
 //    ||     ||  || ||..||    ||       ||    .|''||    \\/\//    ||  ||..||  ||        ||       ||    ||  ||    || ||..|| ||      ||      //
 //   .||.   .||  || `|...     `|....' .||.   `|..||.    \/\/    .||. `|...  .||.      .||      .||.   `|..|'    || `|...  `|..'   `|..'   //
 //                                                                                                                                        //
 //                  An autonomous rover designed and built around a 1/5th Rock Crawler chasis by Prajwal Bhattaram                        //
 //                                                       v.0.2.0-alpha                                                                    //
 //                                                        21.Oct.2014                                                                     //
 // =====================================================================================================================================  //                                                                                                       `..|'                      
  
  //Libraries
  #include <Wire.h>
 
  
  //Definitions
  #define startbyte 0x0F
  #define I2Caddress 0x07
  #define ECHO_TO_SERIAL 1
  
  
  //Sensor variables
  const int pingPin_R = 22;//sets the pin for the PING sensor on the right side of the car
  const int echoPin_R = 23;//sets the pin for the ECHO sensor on the right side of the car
  const int pingPin_C = 24;//sets the pin for the PING sensor in the center side of the car
  const int echoPin_C = 25;//sets the pin for the ECHO sensor on the center side of the car
  const int pingPin_L = 26;//sets the pin for the PING sensor on the left side of the car
  const int echoPin_L = 27;//sets the pin for the ECHO sensor on the left side of the car
  long duration_C, cm_C, duration_R, cm_R, duration_L, cm_L;
  int r;
  
  //TREX MasterSend variables
  int sv[6]={1500,0,0,0,0,0};                          // servo positions: 0 = Not Used
  //int sd[6]={5,10,-5,-15,20,-20};                      // servo sweep speed/direction (Only if sweeping servos are required
  int lmspeed,rmspeed;                                 // left and right motor speed from -255 to +255 (negative value = reverse)
  int ldir=5;                                          // how much to change leading  motor speed each loop (use for motor testing)
  int rdir=5;                                          // how much to change rear motor speed each loop (use for motor testing)
  byte lmbrake,rmbrake;                                // leading and rear motor brake (non zero value = brake)
  byte devibrate=50;                                   // time delay after impact to prevent false re-triggering due to chassis vibration
  int sensitivity=50;                                  // threshold of acceleration / deceleration required to register as an impact
  int lowbat=590;                                      // adjust to suit your battery: 550 = 5.50V
  byte i2caddr=7;                                      // default I2C address of T'REX is 7. If this is changed, the T'REX will automatically store new address in EEPROM
  byte i2cfreq=0;                                      // I2C clock frequency. Default is 0=100kHz. Set to 1 for 400kHz
  //byte dir=0                                         // Indicates direction of travel 0=forward; 1=backward
  int s_no=1;                                          //Indicates number of servos
  
  //TREX MasterReceive variables
  int lmcur,rmcur;                                       // left and right motor current
  int lmenc,rmenc;                                       // left and right encoder values
  int volts;                                             // battery voltage*10 (accurate to 1 decimal place)
  int xaxis,yaxis,zaxis;                                 // X, Y, Z accelerometer readings
  int deltx,delty,deltz;                                 // X, Y, Z impact readings 
  int magnitude;                                         // impact magnitude
  
  void setup()
  {
    #if ECHO_TO_SERIAL
    Serial.begin(115200);
    #endif
    Wire.begin();
  }
  
  void loop()
  { 
    
    //Ping sensors
    int i;
    int j = 1;
    delay (2000); //Can be increased to 5000 if timing is off
    for(i=0; i<1; i*1)
    {
      j++;
      r=j;
      pingAll();
      avoid_obstacles();
      j=r;
    }
    
    // Send data via I2C to TREX
    MasterSend(startbyte,2,lmspeed,lmbrake,rmspeed,rmbrake,sv[0],sv[1],sv[2],sv[3],sv[4],sv[5],devibrate,sensitivity,lowbat,i2caddr,i2cfreq);
    delay(50);
    
    // Receive data packet from T'REX controller
    MasterReceive();                                   
    delay(50);
    
    // Work with received data
    if (volts<591)
    {
      low_voltage();
    }                                                 // Trigger low voltage alert and stop
    
    magnitude=sqrt(sq(deltx)+sq(delty)+sq(deltz));    //If impact detected, calculate the magnitude of impact
    if (magnitude>sensitivity)
    {
      resolve_impact();
    }                                                //Determines cause of impact and takes corrective action
    
    if (lmcur>39000 || rmcur>39000)                  //If motor current --> stall current (The TREX motor controller is rated for 40A stall current)
    {
      stall_prevention();
    }                                                //Prevents motor stall
    
    
    
    /* //=================================================== Code to test motors and sweep servos =============================================  
    lmspeed+=ldir;
    if(lmspeed>240 or lmspeed<-240) ldir=-ldir;        // increase / decrease left motor speed and direction (negative values = reverse direction)
    
    rmspeed+=rdir;
    if(rmspeed>240 or rmspeed<-240) rdir=-rdir;        // increase / decrease left motor speed and direction (negative values = reverse direction)
    
    lmbrake=(abs(lmspeed)>235);                        // test left  motor brake 
    rmbrake=(abs(rmspeed)>235);                        // test right motor brake 
    
    for(byte i=0;i<6;i++)                              // sweep servos
    {
      if(sv[i]!=0)                                     // a value of 0 indicates no servo attached
      {
        sv[i]+=sd[i];                                  // update servo position to create sweeping motion
        if(sv[i]>2000 || sv[i]<1000) sd[i]=-sd[i];     // reverse direction of servo if limit reached
      }
    } */
    
  }

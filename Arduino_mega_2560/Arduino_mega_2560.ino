  
 // ==========================================================================================================================================================================================================================================================================  //
 //                                                                                                                                                                                                                                                                             //
 //                                                             |''||''| '||               .|'''',                           '||`                  '||'''|,                                    ||                                                                               //
 //                                                                ||     ||               ||                                 ||                    ||   ||                  ''                ||                                                                               //
 //                                                                ||     ||''|, .|''|,    ||      '||''|  '''|.  '\\    //`  ||  .|''|, '||''|     ||...|' '||''| .|''|,    || .|''|, .|'', ''||''                                                                             //
 //                                                                ||     ||  || ||..||    ||       ||    .|''||    \\/\//    ||  ||..||  ||        ||       ||    ||  ||    || ||..|| ||      ||                                                                               //
 //                                                               .||.   .||  || `|...     `|....' .||.   `|..||.    \/\/    .||. `|...  .||.      .||      .||.   `|..|'    || `|...  `|..'   `|..'                                                                            //
 //                                                                                                                                                                                                                                                                             //
 //                                                                              An autonomous rover designed and built around a 1/5th Rock Crawler chasis by Prajwal Bhattaram                                                                                                 //
 //                                                                                                                   v.0.4.0-alpha                                                                                                                                             //
 //                                                                                                                    21.Oct.2014                                                                                                                                              //
 //                                                                                                   https://github.com/Marzogh/The-Crawler-Project                                                                                                                            //
 // ==========================================================================================================================================================================================================================================================================  //                                                                                                                             
  
  //Libraries
  #include <Wire.h>                                       // Used by: T-Rex & IMU
  #include <SPI.h>                                        // Used by: GPS
  #include <Adafruit_Sensor.h>                            // Used by: IMU
  #include <Adafruit_LSM9DS0.h>                           // Used by: IMU
  #include <Adafruit_Simple_AHRS.h>                       // Used by: IMU
  #include <NewPing.h>                                    // Used by: Ultrasonics
  #include <Adafruit_GPS.h>                               // Used by: GPS
  #include <SoftwareSerial.h>                             // Used by: GPS (Uncomment if SoftwareSerial is needed)
  #include "Waypoint_class.h"                             // Custom class to manage GPS waypoints
  #include <math.h>                                       // Used by: GPS
  #include "moving_average.h"                             // Simple moving average class; for Sonar functionality
 
  
  //Definitions
  #define startbyte 0x0F
  #define I2Caddress 0x07
  //#define DEBUG                                         //Uncomment for debugging (remember to comment out code in MasterReceive before DEBUG)
  
  
  //Sensor variables
  //Ultrasonics' variables
  int r;
  #define pingPin_R  22
  #define pingPin_C  24 
  #define pingPin_L  26  
  #define echoPin_R  23      
  #define echoPin_C  25
  #define echoPin_L  27  
  #define MAX_DISTANCE_CM 250                            // Maximum distance we want to ping for (in CENTIMETERS). Maximum sensor distance is rated at 400-500cm.  
  //#define MAX_DISTANCE_IN (MAX_DISTANCE_CM / 2.5)      // Same distance, in inches
  int sonarDistance_R;
  int sonarDistance_C;
  int sonarDistance_L;
  NewPing sonarR(pingPin_R, echoPin_R, MAX_DISTANCE_CM); // NewPing setup of pins and maximum distance.
  NewPing sonarC(pingPin_C, echoPin_C, MAX_DISTANCE_CM);
  NewPing sonarL(pingPin_L, echoPin_L, MAX_DISTANCE_CM);
  MovingAverage<int, 3> sonarRAverage(MAX_DISTANCE_CM);  // Moving average of last n pings, initialize at MAX_DISTANCE_CM
  MovingAverage<int, 3> sonarCAverage(MAX_DISTANCE_CM); 
  MovingAverage<int, 3> sonarLAverage(MAX_DISTANCE_CM); 
  
  //IMU variables
  Adafruit_LSM9DS0     lsm(1000);                        // Create LSM9DS0 board instance. Use I2C, ID #1000
  Adafruit_Simple_AHRS ahrs(&lsm.getAccel(), &lsm.getMag()); // Create simple AHRS algorithm using the LSM9DS0 instance's accelerometer and magnetometer.
  // Compass navigation variables
  int targetHeading;                                    // Where we want to go to reach current waypoint
  int currentHeading;                                   // Where we are actually facing now
  int headingError;                                     // Signed (+/-) difference between targetHeading and currentHeading
  #define HEADING_TOLERANCE 5                           // Tolerance +/- (in degrees) within which we don't attempt to turn to intercept targetHeading
  
  //GPS variables
  // If using software serial, enable the line below and change pins to match:
  //SoftwareSerial mySerial(3, 2);
  // If using hardware serial (e.g. Arduino Mega), comment out the above SoftwareSerial line, and enable this line below instead and change the Serial port number to match.
  HardwareSerial mySerial = Serial2;
  Adafruit_GPS GPS(&mySerial);
                                                       // Set to 'true' if you want to debug and listen to the raw GPS sentences. 
  boolean usingInterrupt = false;                      // This keeps track of whether we're using the interrupt (off by default!)
  // GPS navigation variables
  float currentLat,
        currentLong,
        targetLat,
        targetLong,
        homeLat,
        homeLong;
  int distanceToTarget,                                // Current distance to target (current waypoint)
      originalDistanceToTarget;                        // Distance to original waypoint when we started navigating to it
      
      
  //Radio variables
  HardwareSerial radio = Serial;                       // Connect radio to Serial port
  #define COMMANDSIZE 19                               // Defines the size of the command packet
  char commandChar=':';                                // Sets command character
  char terminalChar='\r';                              // Sets end character
  boolean storeString=false;
  char commandRead[COMMANDSIZE+1];                    // Char array to store string from radio
  int command[]={0,0,0,0,0,0};                        // Int array to store parsed output from radio
  int mode=command[2];                                // Sets running mode
      
 
 // Waypoints
  #define WAYPOINT_DIST_TOLERANCE  5                   // Tolerance in meters to waypoint; once within this tolerance, will advance to the next waypoint
  #define NUMBER_WAYPOINTS 5                           // Enter the number of way points here (will run from 0 to (n-1))
  #define NUMBER_HOME_WAYPOINTS 1                      // Reserved for future use where multiple homes could exist and the Crawler has to choose the closest. (will run from 0 to (n-1))
  int waypointNumber = -1;                             // Current waypoint number; will run from 0 to (NUMBER_WAYPOINTS -1); start at -1 and gets initialized during setup()
  waypoint waypointList[NUMBER_WAYPOINTS] = 
  {
    waypoint(30.508302, -97.832624), 
    waypoint(30.508085, -97.832494), 
    waypoint(30.507715, -97.832357), 
    waypoint(30.508422, -97.832760), 
    waypoint(30.508518, -97.832665) 
  };
  waypoint homeLocation[NUMBER_HOME_WAYPOINTS] =       // This is where the home waypoint will be set
  { 
    waypoint(homeLat, homeLong)
  };

 
 // Steering/turning 
  #define TURN_LEFT 1000
  #define TURN_RIGHT 2000
  #define TURN_STRAIGHT 1500
  enum directions {left=1000, right=2000, straight=1500} ;
  directions turnDirection = straight;
  
  
  // Object avoidance distances (in cms)
  #define SAFE_DISTANCE 120
  #define TURN_DISTANCE 60
  #define STOP_DISTANCE 30
  
  
  // Speeds (range: 0 - 255)
  #define FAST_SPEED 225
  #define NORMAL_SPEED 175
  #define TURN_SPEED 125
  #define SLOW_SPEED 75
  int speed = NORMAL_SPEED;
 
  
  //TREX MasterSend variables
  int sv[6]={1500,0,0,0,0,0};                          // servo positions: 0 = Not Used
  //int sd[6]={5,10,-5,-15,20,-20};                      // servo sweep speed/direction (Only if sweeping servos are required
  int lmspeed,rmspeed;                                 // left and right motor speed from -255 to +255 (negative value = reverse)
  //int ldir=5;                                          // how much to change leading  motor speed each loop (use for motor testing)
  //int rdir=5;                                          // how much to change rear motor speed each loop (use for motor testing)
  byte lmbrake,rmbrake;                                // leading and rear motor brake (non zero value = brake)
  byte devibrate=50;                                   // time delay after impact to prevent false re-triggering due to chassis vibration
  int sensitivity=50;                                  // threshold of acceleration / deceleration required to register as an impact
  int lowbat;                                          // stores calculated cut-off voltage
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
  
  //Variables for calculations & debugging
  #define DEBUG false                                    //Set to true if you want to debug the microprocessor code & sensor outputs 
  static byte alternate;
  int battType=1;                                        //Change to match battery type - 1.LiPo/LiIon 2.NiMH
  int cellNo=3;                                          //Change to match no. of cells in battery
  
  
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Interrupt code for GPS begins~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
            
            // Interrupt is called once a millisecond, looks for any new GPS data, and stores it
            SIGNAL(TIMER0_COMPA_vect) {
              char c = GPS.read();                      // If you want to debug, this is a good time to do it!
            #ifdef UDR0
              if (DEBUG)
                if (c) UDR0 = c;                        // Writing direct to UDR0 is much much faster than Serial.print but only one character can be written at a time. 
            #endif
            }
            
            void useInterrupt(boolean v) {
              if (v) {
                OCR0A = 0xAF;                           // Timer0 is already used for millis() - we'll just interrupt somewhere in the middle and call the "Compare A" function above
                TIMSK0 |= _BV(OCIE0A);
                usingInterrupt = true;
              } 
              else 
              {
                TIMSK0 &= ~_BV(OCIE0A);                // Do not call the interrupt function COMPA anymore
                usingInterrupt = false;
              }
            }
            
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Interrupt code for GPS ends~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  

  
  void setup()
  {
    //Calculate cutoff voltage for battery
    if (battType=1)
    {
      lowbat=(cellNo*=3.74);
    }
    
    #if DEBUG
    Serial1.begin(115200);
    #endif
    radio.begin(115200);
    Wire.begin();
    batteryCheck();                                      //Check for low battery
    startSensors();
    setHome();
    
    waitForRadio();                                      //Wait for radio command
    parseCommand();                                      //If radio command has been read, then parse the command
    while (!IO)
    break;  
    
  }
  
  void loop()
  {
    if (mode=0)                                          //Radio commands autonomous navigation
    {
      autoNavigate();
    }
    
    if (mode==1)                                          //Radio commands Return to Home
    {
      returnHome();
    }
    
    if (mode==2)                                          //Radio commands RC navigation
    {
      radioControl();
    }
  }

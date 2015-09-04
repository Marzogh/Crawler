
//Libraries
#include <Wire.h>                                       // Used by: T-Rex & IMU
#include <SabertoothSimplified.h>                       // Used by: Sabertooth 2x12
#include <Servo.h>                                      // Used by: Steering servo
#include <SPI.h>                                        // Used by: GPS
#include <Adafruit_Sensor.h>                            // Used by: IMU
#include <Adafruit_LSM9DS0.h>                           // Used by: IMU
#include <Adafruit_Simple_AHRS.h>                       // Used by: IMU
#include <NewPing.h>                                    // Used by: Ultrasonics
#include <TinyGPS++.h>                                  // Used by: GPS
#include <SoftwareSerial.h>                             // Used by: GPS (Uncomment if SoftwareSerial is needed)
#include "Waypoint_class.h"                             // Custom class to manage GPS waypoints
#include <math.h>                                       // Used by: GPS
#include "moving_average.h"                             // Simple moving average class; for Sonar functionality
#include <EEPROMex.h>                                   // For EEPROM
#include <EEPROMVar.h>                                  // For EEPROM


//Pins, Definitions & Constants
//#define RADIO
#define SERIALECHO
#define waypointTolerance  5             // Tolerance in meters to quest; once within this tolerance, will advance to the next quest
const int servoPin = 9;
const byte engage = 0, disengage = 180;
static const int RXPin = 3, TXPin = 2;     //Connect TX of GPS to 3 and RX of GPS to 2
static const int GPSBaud = 9600;

//Radio variables
HardwareSerial radio = Serial1;                      // Connect radio to Serial port
#define COMMANDSIZE 19                               // Defines the size of the command packet
char commandChar = ':';                              // Sets command character
char terminalChar = '\r';                            // Sets end character
boolean storeString = false;
char commandRead[COMMANDSIZE + 1];                   // Char array to store string from radio
int command[] = {0, 0, 0, 0, 0, 0};                  // Int array to store parsed output from radio
int mode = command[2];                               // Sets running mode

//Location Variables
struct coordinates {                       // Stores quest coordinates
  double latitude;
  double longitude;
};

coordinates Home;
coordinates current;
coordinates quest1;
coordinates quest2;
coordinates quest3;
coordinates quest4;
coordinates quest5;
coordinates target;
int numberOfQuests = 5;                   //Number of quests (Does not include home)

struct addresses {                        // Stores EEPROM addresses for quest coordinates
  int latitude;
  int longitude;
};

addresses homeAddress;
addresses Address1;
addresses Address2;
addresses Address3;
addresses Address4;
addresses Address5;

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

// Steering/turning
#define TURN_LEFT 1
#define TURN_RIGHT 179
#define TURN_STRAIGHT 90
enum directions {left = 1000, right = 2000, straight = 1500} ;
directions turnDirection = straight;
Servo steering;                                      // create servo object to control a servo


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

// Sabertooth variables
SoftwareSerial engine(NOT_A_PIN, 11);                // RX on no pin (unused), TX on pin 11 (to S1).
SabertoothSimplified ST(engine);                     // Use SWSerial as the serial port.
int lmspeed, rmspeed;                                // left and right motor speed from 1 to 255 (0 = halt; 1-127 --> left motor --> full rev-full forward;  128-255 --> right motor --> full rev-full forward)
int s_no = 1;                                        //Indicates number of servos
int sv[6] = {1500, 0, 0, 0, 0, 0};                   // servo positions: 0 = Not Used

//IMU variables
Adafruit_LSM9DS0     lsm(1000);                        // Create LSM9DS0 board instance. Use I2C, ID #1000
Adafruit_Simple_AHRS ahrs(&lsm.getAccel(), &lsm.getMag()); // Create simple AHRS algorithm using the LSM9DS0 instance's accelerometer and magnetometer.
// Compass navigation variables
int targetHeading;                                    // Where we want to go to reach current waypoint
int currentHeading;                                   // Where we are actually facing now
int headingError;                                     // Signed (+/-) difference between targetHeading and currentHeading
#define HEADING_TOLERANCE 5                           // Tolerance +/- (in degrees) within which we don't attempt to turn to intercept targetHeading

//Other variables
unsigned long distanceToTarget;
byte currentQuest, currentQuestAddress;
static byte alternate;
int battType = 1;                                      //Change to match battery type - 1.LiPo/LiIon 2.NiMH
int cellNo = 3;                                        //Change to match no. of cells in battery
int lowbat, volts;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*Objects*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// The TinyGPS++ object
TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);


void setup() {

  if (battType = 1)
  {
    lowbat = (cellNo *= 3.74);
  }

#ifdef SERIALECHO
  Serial.begin(115200);
#endif

#ifdef RADIO
  radio.begin(115200);
#endif

  Wire.begin();
  batteryCheck();                                         //Check for low battery
  configureIMU();
  engine.begin(38400);                                   //If (IO) then start serial comms to the motors
  steering.attach(servoPin);                             // attaches the servo on pin 9 to the servo object
  ss.begin(9600);

  getAddresses();
  currentQuest = EEPROM.read(currentQuestAddress);

#ifdef SERIALECHO
  Serial.print (F("The current quest is: Quest "));
  Serial.println(currentQuest);
#endif

#ifdef RADIO
  waitForRadio();                                        //Wait for radio command
  parseCommand();                                        //If radio command has been read, then parse the command
  while (!IO)
    break;
#endif

}

void loop() {

#ifdef RADIO
  switch (mode) {
    case 0:                                               //Radio commands autonomous navigation
      autoNavigate();
      break;

    case: 1                                                //Radio commands Return to Home
        returnHome();
      break;

    case: 2                                               //Radio commands RC navigation
        radioControl();
      break;

    default:
      autonavigate();
      break;
#endif

#ifndef RADIO
      autoNavigate();
#endif

  }

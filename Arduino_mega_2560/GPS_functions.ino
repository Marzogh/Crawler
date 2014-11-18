
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                                                                                     //
//                                                                                   If you're using a GPS module:                                                                                     //
//                                                                                   Connect the GPS Power pin to 5V                                                                                   //
//                                                                                 Connect the GPS Ground pin to ground                                                                                //
//                                                                                                                                                                                                     //
//                                                                           If using software serial (sketch example default):                                                                        //
//                                                                             Connect the GPS TX (transmit) pin to Digital 3                                                                          //
//                                                                              Connect the GPS RX (receive) pin to Digital 2                                                                          //
//                                                                                                                                                                                                     //
//                                                                             If using hardware serial (e.g. Arduino Mega):                                                                           //
//                                                                      Connect the GPS TX (transmit) pin to Arduino RX1, RX2 or RX3                                                                   //
//                                                                      Connect the GPS RX (receive) pin to matching TX1, TX2 or TX3                                                                   //
//                                                                                                                                                                                                     //
//                                                               Code borrowed from the excellent TinyGPS library (with minor modifications)                                                           //
//                                                                                http://arduiniana.org/libraries/tinygps/                                                                             //
//                                                                                                                                                                                                     //
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void useInterrupt(boolean);                                                     // Func prototype keeps Arduino 0023 happy

void configureGPS()  
{
  #if DEBUG
  Serial1.println("Adafruit GPS library basic test!");
  #endif
  
  GPS.begin(9600); //Usually the default baud rate for Adafruit's GPS
  
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);                                // Uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude.
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);                             // Uncomment this line to turn on only the "minimum recommended" data.
                                                                               // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since the parser doesn't care about other sentences at this time.
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);                                   // Set the update rate to 1 Hz (recommended for the parsing code to work properly)
  //GPS.sendCommand(PGCMD_ANTENNA);                                              // Request updates on antenna status, comment out to keep quiet

  useInterrupt(true);                                                          // The nice thing about this code is you can have a timer0 interrupt go off every 1 millisecond, and read data from the GPS for you. That makes the
                                                                               // loop code a heck of a lot easier!
  delay(1000);
  //mySerial.println(PMTK_Q_RELEASE);                                            // Ask for firmware version
  
  //Wait for GPS to get a fix
  unsigned long startTime = millis();
  while (!GPS.fix)                                                             // Wait for fix
    {
      if (GPS.newNMEAreceived())
        GPS.parse(GPS.lastNMEA());
    }
}



void readGPS()                                                               // Run over and over again
{
  uint32_t timer = millis();
  if (! usingInterrupt)                                                      // In case you are not using the interrupt above, you'll need to 'hand query' the GPS (Not recommended)        
  {
    char c = GPS.read();                                                     // Read data from the GPS in the 'main loop'
    #if DEBUG                                                               // If you want to debug, this is a good time to do it!
      if (c) Serial1.print(c);
    #endif
  }
  
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {                                              // A tricky thing here is if we print the NMEA sentence, or data we end up not listening and catching other sentences! 
                                                                            // So be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());                                       // This sets the newNMEAreceived() flag to false
  
    if (!GPS.parse(GPS.lastNMEA()))                                         // This sets the newNMEAreceived() flag to false
      return;                                                               // We can fail to parse a sentence in which case we should just wait for another
    processGPS();
  }

  if (timer > millis())  timer = millis();                                  // If millis() or timer wraps around, reset it

  if (millis() - timer > 2000)                                              // Approximately every 2 seconds or so, print out the current stats
  { 
    #if DEBUG
    {
      timer = millis(); // Reset the timer
      
      Serial.print("\nTime: ");
      Serial.print(GPS.hour, DEC); Serial.print(':');
      Serial.print(GPS.minute, DEC); Serial.print(':');
      Serial.print(GPS.seconds, DEC); Serial.print('.');
      Serial.println(GPS.milliseconds);
      Serial.print("Date: ");
      Serial.print(GPS.day, DEC); Serial.print('/');
      Serial.print(GPS.month, DEC); Serial.print("/20");
      Serial.println(GPS.year, DEC);
      Serial.print("Fix: "); Serial.print((int)GPS.fix);
      Serial.print(" quality: "); Serial.println((int)GPS.fixquality); 
      if (GPS.fix) 
      {
        Serial.print("Location: ");
        Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
        Serial.print(", "); 
        Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
        Serial.print("Location (in degrees, works with Google Maps): ");
        Serial.print(GPS.latitudeDegrees, 4);
        Serial.print(", "); 
        Serial.println(GPS.longitudeDegrees, 4);
        
        Serial.print("Speed (knots): "); Serial.println(GPS.speed);
        Serial.print("Angle: "); Serial.println(GPS.angle);
        Serial.print("Altitude: "); Serial.println(GPS.altitude);
        Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      }
    }
    #endif
  }
}



void nextWaypoint(void)
{
  if (mode==0 || mode==2)
  {
    waypointNumber++;
    targetLat = waypointList[waypointNumber].getLat();
    targetLong = waypointList[waypointNumber].getLong();
    
    if ((targetLat == 0 && targetLong == 0) || waypointNumber >= NUMBER_WAYPOINTS)    // last waypoint reached? 
      {
        halt();    // make sure we stop
        centerServos();
        loopForever();
      }
      
     processGPS();
     distanceToTarget = originalDistanceToTarget = distanceToWaypoint();
     courseToWaypoint();
  }
  if (mode==1)
  {
    int homeWaypointNumber = -1;
    homeWaypointNumber++;
    targetLat = homeLocation[homeWaypointNumber].getLat();
    targetLong = homeLocation[homeWaypointNumber].getLong();
    
    if ((targetLat == 0 && targetLong == 0) || homeWaypointNumber >= NUMBER_HOME_WAYPOINTS)    // last waypoint reached? 
      {
        halt();    // make sure we stop
        centerServos();
        loopForever();
      }
      
     processGPS();
     distanceToTarget = originalDistanceToTarget = distanceToWaypoint();
     courseToWaypoint();
  }
}

// Returns distance in meters between two positions, both specified as signed decimal-degrees latitude and longitude. Uses great-circle 
// distance computation for hypothetical sphere of radius 6372795 meters. Earth is not an exact sphere, so, rounding errors may be up to 0.5%.
int distanceToWaypoint() 
{
  
  float delta = radians(currentLong - targetLong);
  float sdlong = sin(delta);
  float cdlong = cos(delta);
  float lat1 = radians(currentLat);
  float lat2 = radians(targetLat);
  float slat1 = sin(lat1);
  float clat1 = cos(lat1);
  float slat2 = sin(lat2);
  float clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong); 
  delta = sq(delta); 
  delta += sq(clat2 * sdlong); 
  delta = sqrt(delta); 
  float denom = (slat1 * slat2) + (clat1 * clat2 * cdlong); 
  delta = atan2(delta, denom); 
  distanceToTarget =  delta * 6372795; 
   
  // check to see if we have reached the current waypoint
  if (distanceToTarget <= WAYPOINT_DIST_TOLERANCE)
    nextWaypoint();
    
  return distanceToTarget;
}



// Returns course in degrees (North=0, West=270) from position 1 to position 2, both specified as signed decimal-degrees latitude and longitude.
// Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
int courseToWaypoint() 
{
  float dlon = radians(targetLong-currentLong);
  float cLat = radians(currentLat);
  float tLat = radians(targetLat);
  float a1 = sin(dlon) * cos(tLat);
  float a2 = sin(cLat) * cos(tLat) * cos(dlon);
  a2 = cos(cLat) * sin(tLat) - a2;
  a2 = atan2(a1, a2);
  if (a2 < 0.0)
  {
    a2 += TWO_PI;
  }
  targetHeading = degrees(a2);
  return targetHeading;
}   // courseToWaypoint()





// Converts lat/long from Adafruit degree-minute format to decimal-degrees; requires <math.h> library
double convertDegMinToDecDeg (float degMin) 
{
  double min = 0.0;
  double decDeg = 0.0;
 
  //Get the minutes, fmod() requires double
  min = fmod((double)degMin, 100.0);
 
  //Rebuild coordinates in decimal degrees
  degMin = (int) ( degMin / 100 );
  decDeg = degMin + ( min / 60 );
 
  return decDeg;
}



void processGPS(void)
{
  currentLat = convertDegMinToDecDeg(GPS.latitude);
  currentLong = convertDegMinToDecDeg(GPS.longitude);
             
  if (GPS.lat == 'S')            // make them signed
    currentLat = -currentLat;
  if (GPS.lon = 'W')  
    currentLong = -currentLong; 
             
  // Update the course and distance to waypoint based on our new position
  distanceToWaypoint();
  courseToWaypoint();
}

void setHome()
{
  readGPS();
  
  //convert lat/long from degree-min to decimal-degrees; requires <math.h> library
  currentLat = convertDegMinToDecDeg(GPS.latitude);
  currentLong = convertDegMinToDecDeg(GPS.longitude);
             
  if (GPS.lat == 'S')            // make them signed
    homeLat = -homeLat;
  if (GPS.lon = 'W')  
    homeLong = -homeLong;
}
  

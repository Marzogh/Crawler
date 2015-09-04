
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

// This custom version of delay() ensures that the gps object is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}


void nextWaypoint(void)
{
  switch (mode) {

    case 0:
      EEPROM.updateByte(currentQuestAddress, currentQuest++);
      currentQuest = EEPROM.readByte(currentQuestAddress);
      break;

    case 1:
      EEPROM.updateByte(currentQuestAddress, 0);
      currentQuest = EEPROM.readByte(currentQuestAddress);
      break;

    case 2:

      break;

    default:

      break;
  }
      updateTarget();
      distanceToWaypoint();
      courseToWaypoint();
  }

  // Returns distance in meters between two positions, both specified as signed decimal-degrees latitude and longitude. Uses great-circle
  // distance computation for hypothetical sphere of radius 6372795 meters. Earth is not an exact sphere, so, rounding errors may be up to 0.5%.
  void distanceToWaypoint()
  {

    float delta = radians(current.longitude - target.longitude);
    float sdlong = sin(delta);
    float cdlong = cos(delta);
    float lat1 = radians(current.latitude);
    float lat2 = radians(target.latitude);
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
    if (distanceToTarget <= waypointTolerance) {
      if (currentQuest > numberOfQuests || currentQuest == 0) {
        halt();
        centerServos();
        while (1);
      }
      else
        nextWaypoint();
    }

    //return distanceToTarget;
  }



  // Returns course in degrees (North=0, West=270) from position 1 to position 2, both specified as signed decimal-degrees latitude and longitude.
  // Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
  void courseToWaypoint()
  {
    float dlon = radians(target.longitude - current.longitude);
    float cLat = radians(current.latitude);
    float tLat = radians(target.latitude);
    float a1 = sin(dlon) * cos(tLat);
    float a2 = sin(cLat) * cos(tLat) * cos(dlon);
    a2 = cos(cLat) * sin(tLat) - a2;
    a2 = atan2(a1, a2);
    if (a2 < 0.0)
    {
      a2 += TWO_PI;
    }
    targetHeading = degrees(a2);
    //return targetHeading;
  }   // courseToWaypoint()



  void processGPS(void)
  {
    current.latitude = gps.location.lat();
    current.longitude = gps.location.lng();

    // Update the course and distance to waypoint based on our new position
    distanceToWaypoint();
    courseToWaypoint();
  }


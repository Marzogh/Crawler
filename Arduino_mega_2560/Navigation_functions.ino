  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  //                                                                                                                                                                                                                                    //
  //                                                                                                   Functions for navigation                                                                                                         //
  //                                                                                                                                                                                                                                    //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  
  void autoRoute()
  {
    if(GPS.parse(GPS.lastNMEA()) )
      processGPS();  
    readIMU();                          // get our current heading
    calcDesiredTurn();                  // calculate how we would optimatally turn, without regard to obstacles      
    readSonar();
    delay(100);
  }
   
  void pickCourse()
  {
    if (headingError <= 0)
    turnDirection = left;
    else
    turnDirection = right;setServos();                                                            // turn in the new direction
    updateTREX();
  }
  
   void readSensors()
   {
     readIMU();
     readGPS();
     calcDesiredTurn();                // calculate how we would optimatally turn, without regard to obstacles  `
     readSonar();
   }
   
  void calcDesiredTurn(void)
  {
      // calculate where we need to turn to head to destination
      headingError = targetHeading - currentHeading;
      
      // adjust for compass wrap
      if (headingError < -180)      
        headingError += 360;
      if (headingError > 180)
        headingError -= 360;
    
      // calculate which way to turn to intercept the targetHeading
      if (abs(headingError) <= HEADING_TOLERANCE)      // if within tolerance, don't turn
        turnDirection = straight;  
      else if (headingError < 0)
        turnDirection = left;
      else if (headingError > 0)
        turnDirection = right;
      else
        turnDirection = straight;
   
  }  // calcDesiredTurn()


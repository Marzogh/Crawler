
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                                                                                                                                                                                                                                    //
//                                                                                                   Functions for navigation                                                                                                         //
//                                                                                                                                                                                                                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void autoRoute()
{
  readIMU();                          // get our current heading
  calcDesiredTurn();                  // calculate how we would optimatally turn, without regard to obstacles
  readSonar();
  smartDelay(50);
  processGPS();
}

void pickCourse()
{
  if (headingError <= 0)
    turnDirection = left;
  else
    turnDirection = right;
  setServos();                                                            // turn in the new direction
  //updateTREX();
}

void readSensors()
{
  readSonar();
  readIMU();
  smartDelay(100);
  calcDesiredTurn();                // calculate how we would optimally turn, without regard to obstacles  `
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



void autoNavigate()
{
#if DEBUG
  if (currentQuest == 0)
    Serial1.println("Returning home...");
#endif
  readSonar();
  moveAndDodge();
  //updateTREX();
  smartDelay(50);

#ifdef RADIO
  if (alternate)
  {

    parseCommand();                                    //Check Radio for any updates
    if (!IO)
    {
      halt();                                          // Make sure we stop
      centerServos();                                  // Center Servos
      loopForever();                                   // Keep looping till IO = true
    }
  }
  else
  {
    /* // Receive data packet from T'REX controller
    MasterReceive();
    smartDelay(50);
    // Work with received data
    magnitude=sqrt(sq(deltx)+sq(delty)+sq(deltz));    //If impact detected, calculate the magnitude of impact

    batteryCheck();                                   //Check battery voltage and turn off if voltage = cutoff

    if (lmcur>30000 || rmcur>30000)                   //If motor current --> stall current (The TREX motor controller is rated for 40A stall current)
      stall_prevention();                             //Prevents motor stall

    if (magnitude>sensitivity)
      resolve_impact();                               //Determines cause of impact and takes corrective action */
  }
  alternate = alternate ^ 1;
#endif
}

#ifdef RADIO
void radioControl()
{
#if DEBUG
  Serial1.println("Switched to Radio Control");
#endif
  lmspeed, rmspeed = command[3];
  //lmbrake, rmbrake = command[4];                     //Only to be used for TREX
  for (byte i = 0; i < s_no; i++)                    // Max value of i should be < total number of servos
  {
    if (sv[i] != 0)                                  // If servo is attached
      sv[i] = command[5];                              // Set servo direction
  }
  updateSabertooth();
  parseCommand();

  if (!IO)
  {
    halt();                                          // Make sure we stop
    centerServos();                                  // Center Servos
    loopForever();                                   // Keep looping till IO = true
  }
}
#endif

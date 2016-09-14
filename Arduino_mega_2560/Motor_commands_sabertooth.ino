

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//                                                                                                                                                                                                                                    //
//                                                                                           Functions that control the motors on the Crawler                                                                                         //
//                                                                                                                                                                                                                                    //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void forward()
{                                              // Turns on drive motor in forward and leaves it on
  if (turnDirection == straight)
    lmspeed, rmspeed = FAST_SPEED;
  else
    lmspeed, rmspeed = TURN_SPEED;
  //lmbrake, rmbrake=0;                       // Turns off brakes
  setServos();
  updateSabertooth();
}

void backward()
{                                              // Turns on drive motor in reverse and leaves it on
  turnDirection = straight;
  lmspeed, rmspeed = -NORMAL_SPEED;
  //lmbrake, rmbrake=0;                       // Turns off brakes
  setServos();
  updateSabertooth();
}

void turnRight()
{
  turnDirection = right;
  lmspeed, rmspeed = TURN_SPEED;
  //lmbrake, rmbrake=0;                       // Turns off brakes
  setServos();
  updateSabertooth();
}

void turnRightRev()
{
  turnDirection = left;
  lmspeed, rmspeed = TURN_SPEED;
  //lmbrake, rmbrake=0;                       // Turns off brakes
  setServos();
  updateSabertooth();
}

void turnLeft()
{
  turnDirection = left;
  lmspeed, rmspeed = TURN_SPEED;
  //lmbrake, rmbrake=0;                       // Turns off brakes
  setServos();
  updateSabertooth();
}

void turnLeftRev()
{
  turnDirection = right;
  lmspeed, rmspeed = TURN_SPEED;
  //lmbrake, rmbrake=0;                       // Turns off brakes
  setServos();
  updateSabertooth();
}

void turnStraight()
{
  turnDirection = straight;
  lmspeed, rmspeed = NORMAL_SPEED;
  //lmbrake, rmbrake=0;                       // Turns off brakes
  setServos();
  updateSabertooth();
}

void halt()                                    // Brakes and stops both motors
{
  lmspeed=0;
  rmspeed=0;
  updateSabertooth();
}

void setServos()
{
  for(byte i=0;i<s_no;i++)                  // Max value of i should be < total number of servos
  {
    if(sv[i]!=0)                            // If servo is attached
    {
      if (turnDirection == straight)
        //sv[i]=TURN_STRAIGHT;              //Uncomment for TREX
        steering.write(TURN_STRAIGHT);
      else if (turnDirection == left)
        //sv[i]=TURN_LEFT;                  //Uncomment for TREX
        steering.write(TURN_LEFT);
      else if (turnDirection == right)
        //sv[i]=TURN_RIGHT;                 //Uncomment for TREX
        steering.write(TURN_RIGHT);
    }
  }
}

void centerServos()
{
  for(byte i=0;i<s_no;i++)                 // Max value of i should be < total number of servos
  {
    if(sv[i]!=0)                           // If servo is attached
      //sv[i]=TURN_STRAIGHT;              //Uncomment for TREX
      steering.write(TURN_STRAIGHT);
  }
}

void slowDown()
{
  lmspeed, rmspeed = SLOW_SPEED;
  //lmbrake, rmbrake=0;                       // Turns off brakes
  updateSabertooth();
}

void reduceSpeed()
{
  lmspeed, rmspeed = NORMAL_SPEED;
  //lmbrake, rmbrake=0;                       // Turns off brakes
  updateSabertooth();
}

int mapToSabertooth(int side, int spd)
{
  if (side == 1)
  {
    if (spd <= -1)
    {
      spd = map(spd, -255, -1, 1, 63);
      spd = constrain(spd, 1, 63);
      return spd;
    }
    if (spd >=1)
    {
      spd = map(spd, 1, 255, 65, 127);
      spd = constrain(spd, 65, 127);
      return spd;
    }
    if (spd = 0)
      spd = 64;

  }

  if (side == 2)
  {
    if (spd <= -1)
    {
      spd = map(spd, -255, -1, 128, 191);
      spd = constrain(spd, 128, 191);
      return spd;
    }
    if (spd >=1)
    {
      spd = map(spd, 1, 255, 193, 255);
      spd = constrain(spd, 193, 255);
      return spd;
    }
    if (spd = 0)
      spd = 192;
  }
  return spd;
}

void updateSabertooth()
{
  mapToSabertooth(1, lmspeed);
  mapToSabertooth(2, rmspeed);
  ST.motor(1, lmspeed);
  ST.motor(2, rmspeed);
}


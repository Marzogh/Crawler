    
  
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
       lmbrake, rmbrake=0;                         // Turns off brakes
       updateTREX();
     }
  
  void backward()
     {                                              // Turns on drive motor in reverse and leaves it on
       turnDirection = straight;
       lmspeed, rmspeed = -NORMAL_SPEED;
       lmbrake, rmbrake=0;                         // Turns off brakes
       setServos();
       updateTREX();
     }
  
  void turnRight()
     {
       turnDirection = right;
       lmspeed, rmspeed = TURN_SPEED;
       lmbrake, rmbrake=0;                         // Turns off brakes
       setServos();
       updateTREX();
     }
     
  void turnRightRev()
     {
       turnDirection = right;
       lmspeed, rmspeed = TURN_SPEED;
       lmbrake, rmbrake=0;                         // Turns off brakes
       setServos();
       updateTREX();
     }
  
  void turnLeft()
     {
       turnDirection = left;
       lmspeed, rmspeed = TURN_SPEED;
       lmbrake, rmbrake=0;                         // Turns off brakes
       setServos();
       updateTREX();
     }
     
  void turnLeftRev()
     {
       turnDirection = left;
       lmspeed, rmspeed = TURN_SPEED;
       lmbrake, rmbrake=0;                         // Turns off brakes
       setServos();
       updateTREX();
     }
     
  void turnStraight()
  {
      turnDirection = straight;
      lmspeed, rmspeed = NORMAL_SPEED;
      lmbrake, rmbrake=0;                         // Turns off brakes
      setServos();
      updateTREX();
  }
  
  void halt()                                    // Brakes and stops both motors
    {
      lmbrake=(abs(lmspeed));
      rmbrake=(abs(rmspeed));
      updateTREX();
     }
     
  void setServos()
    {
      for(byte i=0;i<s_no;i++)                  // Max value of i should be < total number of servos
        {
          if(sv[i]!=0)                          // If servo is attached
          {
            if (turnDirection == straight)
              sv[i]=TURN_STRAIGHT;
            else if (turnDirection == left)
              sv[i]=TURN_LEFT;
            else if (turnDirection == right)
              sv[i]=TURN_RIGHT;
          }
        }
    }
    
  void centerServos()
    {
      for(byte i=0;i<s_no;i++)                 // Max value of i should be < total number of servos
        {
          if(sv[i]!=0)                         // If servo is attached
            sv[i]=TURN_STRAIGHT;
        }
    }
    
  void slowDown()
    {
      lmspeed, rmspeed = SLOW_SPEED;
      lmbrake, rmbrake=0;                       // Turns off brakes
      updateTREX();
    }
    
  void reduceSpeed()
    {
      lmspeed, rmspeed = NORMAL_SPEED;
      lmbrake, rmbrake=0;                       // Turns off brakes
      updateTREX();
    }

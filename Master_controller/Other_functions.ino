
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  //                                                                                                                                                                                                                                    //
  //                                                                                                       Miscellaneous functions                                                                                                      //
  //                                                                                                                                                                                                                                    //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// React to low voltage
void batteryCheck()
{
  if (volts <= lowbat)
      {
        halt();
        //beep();
        while(1);
      }                                                 // Trigger low voltage alert and stop
      
}

// Resolve an impact
void resolve_impact()
{
     halt();
     backward();
     smartDelay(1500);
     halt();
       if(r%3 == 0) //This if/else statement is designed to build a little "randomness"
       { 
       turnRightRev(); //into the robot's movements, so it is less likely to become stuck
       smartDelay(750);
       halt();
       turnLeft();
       smartDelay(750);
       turnStraight();
       r=1;
       }
     else
     {
       backward();
       turnLeftRev();
       turnLeftRev();
       halt();
       turnRight();
       smartDelay(750);
       turnStraight();
       }
}

// Prevent motor stall
void stall_prevention()
{
  halt();
  smartDelay(1500);
  turnStraight();
}


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  //                                                                                                                                                                                                                                    //
  //                                                                                                       Miscellaneous functions                                                                                                      //
  //                                                                                                                                                                                                                                    //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

 void startSensors()
 {  
  // Setup the sensor gain and integration time.
  configureIMU();
  //Setup GPS
  configureGPS();                                      //Configures GPS
 }
 
// React to low voltage
void batteryCheck()
{
  if (volts=lowbat)
      {
        halt();
        //beep();
        loopForever();
      }                                                 // Trigger low voltage alert and stop
      
}

// Resolve an impact
void resolve_impact()
{
     halt();
     backward();
     delay(1500);
     halt();
       if(r%3 == 0) //This if/else statement is designed to build a little "randomness"
       { 
       turnRightRev(); //into the robot's movements, so it is less likely to become stuck
       delay(750);
       halt();
       turnLeft();
       delay(750);
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
       delay(750);
       turnStraight();
       }
}

// Prevent motor stall
void stall_prevention()
{
  halt();
  delay(1500);
  turnStraight();
}

// End of program routine, loops forever
void loopForever(void)
{
  while (1);
}

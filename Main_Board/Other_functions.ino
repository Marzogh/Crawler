
// React to low voltage
void low_voltage()
{
  stop();
  beep();
}

// Resolve an impact
void resolve_impact()
{
     stop();
     backward();
     delay(1500);
     stop();
       if(r%3 == 0) //This if/else statement is designed to build a little "randomness"
       { 
       backward();
       right(); //into the robot's movements, so it is less likely to become stuck
       right(); //in a loop, performing the same actions over and over which only.
       stop();
       left();
       delay(750);
       forward();
       r=1;
       }
     else
     {
       backward();
       left();
       left();
       stop();
       right();
       delay(750);
       forward();
       }
}

// Prevent motor stall
void stall_prevention()
{
  stop();
  delay(1500);
  forward();
}

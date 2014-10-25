  
void avoid_obstacles()
{
   //all clear, no obstacles within 250cm in any direction
    if(cm_C >= 250 && cm_R >= 250 && cm_L >= 250)
    {
     straight();
     forward();
    }
   
  //obstacle(s) within 0-30cm range
    else if (cm_L < 30 || cm_C < 30 || cm_R < 30)
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
       delay(500);
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
       delay(500);
       forward();
       }
    }
   
  //obstacle(s) within 30cm-60cm range
   
    //obstacle on left and center and right
    else if (cm_R < 60 && cm_C < 60 && cm_L < 60)
    {
     stop();
     backward();
     delay(1500);
     left();
     delay(500);
     stop();
     right();
     forward();
    }
    //obstacle on center OR left and right
    else if (cm_L >= 60 && cm_R >= 60 && cm_C < 60 ||
              cm_C >= 60 && cm_R < 60 && cm_L < 60)
    {
     stop();
     backward();
     delay(1500);
     left();
     delay(500);
     stop();
     right();
     forward();
    }
    //obstacle on left and center
    else if (cm_R >= 60 && cm_C < 60 && cm_L < 60)
    {
     stop();
     backward();
     delay(1500);
     left();
    }
    //obstacle on right and center
    else if (cm_L >= 60 && cm_C < 60 && cm_R < 60)
    {
     stop();
     backward();
     delay(1500);
     right();
    }
    //obstacle on right
    else if (cm_L >= 60 && cm_C >= 60 && cm_R < 60)
    {
     left();
     left();
    }
    //obstacle on left
    else if (cm_R >= 60 && cm_C >= 60 && cm_L < 60)
    {
     right();
     right();
    }
   
  //obstacle(s) within 60cm-120cm range
   
    //obstacle on left and center
    else if (cm_R >= 120 && cm_C < 120 && cm_L < 120)
    {
     right();
     right();
    }
    //obstacle on right and center
    else if (cm_L >= 120 && cm_C < 120 && cm_R < 120)
    {
     left();
     left();
    }
    //obstacle on right and left
    else if (cm_C >= 120 && cm_L < 120 && cm_R < 120)
    {
     forward();
    }
    //obstacle on right
    else if (cm_L >= 120 && cm_C >= 120 && cm_R < 120)
    {
     left();
    }
    //obstacle on left
    else if (cm_R >= 120 && cm_C >= 120 && cm_L < 120)
    {
     right();
    }
    //obstacle on center
    else if (cm_L >= 120 && cm_R >= 120 && cm_C < 120)
    {
     if(r%2 == 0){
       left();
       r=1;
     }
     else
     {
       right();
      }
    }
}

  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  //                                                                                                                                                                                                                                    //
  //                                                                                               Function for obstacle avoidance                                                                                                      //
  //                                                                                                                                                                                                                                    //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  
void moveAndDodge()
{
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
  //                                                                                                                                                                                               //
  //                                                                                No obstacle(s) within 250cm range                                                                              //
  //                                                                                                                                                                                               //
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
    if(sonarDistance_C > SAFE_DISTANCE && sonarDistance_R > SAFE_DISTANCE && sonarDistance_L > SAFE_DISTANCE)
    {
      forward();
    }
   
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
  //                                                                                                                                                                                               //
  //                                                                                Obstacle(s) within 30cm range                                                                                  //
  //                                                                                                                                                                                               //
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
    else if (sonarDistance_L < STOP_DISTANCE || sonarDistance_C < STOP_DISTANCE || sonarDistance_R < STOP_DISTANCE)
    {
     halt();
     backward();
     while (sonarDistance_C < TURN_DISTANCE)                                       // backup until we get safe clearance
       autoRoute();                                                                // while (sonarDistance < TURN_DISTANCE)
     halt();                                                                       // stop backing up
    }
   
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
  //                                                                                                                                                                                               //
  //                                                                                Obstacle(s) within 30cm-60cm range                                                                             //
  //                                                                                                                                                                                               //
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
   
    //Obstacle on left and center and right
    else if (sonarDistance_R < TURN_DISTANCE && sonarDistance_C < TURN_DISTANCE && sonarDistance_L < TURN_DISTANCE)
    {
      slowDown();
      switch (turnDirection)
      {
        case straight:                                                            // going straight currently, so start new turn
        {
          pickCourse();
          break;
          }
          case left:                                                              // if already turning left, try right
          {
            turnRight();
            break;
          }
          case right:                                                            // if already turning right, try left
          {
            turnLeft();
            break;
            }
          } // end SWITCH
        
      }
      
      //Obstacle on center OR left and right
      else if (sonarDistance_L >= TURN_DISTANCE && sonarDistance_R >= TURN_DISTANCE && sonarDistance_C < TURN_DISTANCE || sonarDistance_C >= TURN_DISTANCE && sonarDistance_R < TURN_DISTANCE && sonarDistance_L < TURN_DISTANCE)
    {
      slowDown();
      switch (turnDirection)
      {
        case straight:                                                          // going straight currently, so start new turn
        {
          pickCourse();
          break;
        }
        case left:                                                             // if already turning left, try right
        {
          turnRight();
          break;
        }
        case right:                                                           // if already turning right, try left
        {
          turnLeft();
          break;
        }
      } // end SWITCH
    }
    
    //Obstacle on left and center
    else if (sonarDistance_R >= TURN_DISTANCE && sonarDistance_C < TURN_DISTANCE && sonarDistance_L < TURN_DISTANCE)
    {
     turnRight();
     while (sonarDistance_L < TURN_DISTANCE)                                 // Keep turning until we get safe clearance
       autoRoute();                                                          // while (sonarDistance < TURN_DISTANCE)
    }
    
    //Obstacle on right and center
    else if (sonarDistance_L >= TURN_DISTANCE && sonarDistance_C < TURN_DISTANCE && sonarDistance_R < TURN_DISTANCE)
    {
     turnLeft();
     while (sonarDistance_R < TURN_DISTANCE)                               // Keep turning until we get safe clearance
       autoRoute();                                                        // while (sonarDistance < TURN_DISTANCE)
    }
    
    //Obstacle on right
    else if (sonarDistance_L >= TURN_DISTANCE && sonarDistance_C >= TURN_DISTANCE && sonarDistance_R < TURN_DISTANCE)
    {
     turnLeft();
     while (sonarDistance_R < TURN_DISTANCE)                             // Keep turning until we get safe clearance
       autoRoute();                                                      // while (sonarDistance < TURN_DISTANCE)
    }
    
    //Obstacle on left
    else if (sonarDistance_R >= TURN_DISTANCE && sonarDistance_C >= TURN_DISTANCE && sonarDistance_L < TURN_DISTANCE)
    {
     turnRight();
     while (sonarDistance_R < TURN_DISTANCE)                             // Keep turning until we get safe clearance
       autoRoute();                                                      // while (sonarDistance < TURN_DISTANCE)
    }
  
   
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
  //                                                                                                                                                                                               //
  //                                                                                Obstacle(s) within 60cm-120cm range                                                                            //
  //                                                                                                                                                                                               //
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
   
    //Obstacle on left and center
    else if (sonarDistance_R >= SAFE_DISTANCE && sonarDistance_C < SAFE_DISTANCE && sonarDistance_L < SAFE_DISTANCE)
    {
      switch (turnDirection)
      {
        case straight:                                                   // going straight currently, so start new turn right
        {
          turnRight();
          while (sonarDistance_L < TURN_DISTANCE)                       // Keep turning until we get safe clearance
            autoRoute();                                                // while (sonarDistance < TURN_DISTANCE)
        }
       case left:                                                       // if already turning left, turn right
       {
         turnRight();
         while (sonarDistance_L < TURN_DISTANCE)                       // Keep turning until we get safe clearance
           autoRoute();                                                // while (sonarDistance < SAFE_DISTANCE) 
       }
       case right:                                                     // if already turning right, stay on course
       {
          turnRight(); //Programmer's note: Not sure if this is needed. Check.
          break;
       }
     } // end SWITCH
   }
    
    //Obstacle on right and center
    else if (sonarDistance_L >= SAFE_DISTANCE && sonarDistance_C < SAFE_DISTANCE && sonarDistance_R < SAFE_DISTANCE)
    {
      switch (turnDirection)
      {
        case straight:                                                 // going straight currently, so start new turn
        {
          turnLeft();
          while (sonarDistance_R < TURN_DISTANCE)                     // Keep turning until we get safe clearance
            autoRoute();                                              // while (sonarDistance < SAFE_DISTANCE)
           break;
        }
       case left:                                                     // if already turning left, stay on course
       {
         turnLeft();
         break; 
       }
       case right:                                                    // if already turning right, turn left
       {
          turnLeft();
          while (sonarDistance_R < TURN_DISTANCE)                    // Keep turning until we get safe clearance
            autoRoute();                                             // while (sonarDistance < SAFE_DISTANCE)
       }
     } // end SWITCH
   }
   
    //Obstacle on right and left
    else if (sonarDistance_C >= SAFE_DISTANCE && sonarDistance_L < SAFE_DISTANCE && sonarDistance_R < SAFE_DISTANCE)
    {
      switch (turnDirection)
      {
        case straight:                                               // Going straight currently, so start new turn
        {
          reduceSpeed();
          break;
        }
       case left:                                                    // If already turning left, try right
       {
         reduceSpeed(); 
         break; 
       }
       case right:                                                  // If already turning right, try left
       {
         reduceSpeed(); 
         break;
       }
          } // end SWITCH
    }
    
    //Obstacle on right
    else if (sonarDistance_L >= SAFE_DISTANCE && sonarDistance_C >= SAFE_DISTANCE && sonarDistance_R < SAFE_DISTANCE)
    {
     reduceSpeed();                                                
    }
    
    //Obstacle on left
    else if (sonarDistance_R >= SAFE_DISTANCE && sonarDistance_C >= SAFE_DISTANCE && sonarDistance_L < SAFE_DISTANCE)
    {
     reduceSpeed(); 
    }
    
    //Obstacle on center
    else if (sonarDistance_L >= SAFE_DISTANCE && sonarDistance_R >= SAFE_DISTANCE && sonarDistance_C < SAFE_DISTANCE)
    {
     if(r%2 == 0)
     {
       turnLeft();
       while (sonarDistance_R < SAFE_DISTANCE)                      // Keep turning until we get safe clearance
            autoRoute(); 
    }
       r=1;
     }
     else
     {
       turnRight();
       while (sonarDistance_L < SAFE_DISTANCE)                      // Keep turning until we get safe clearance
            autoRoute(); 
    }
}

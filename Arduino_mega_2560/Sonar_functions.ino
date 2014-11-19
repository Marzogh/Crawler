
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  //                                                                                                                                                                                                                                    //
  //                                                                                         Function to read and interpret Sonar values                                                                                                //
  //                                                                                                                                                                                                                                    //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

   void readSonar(void)
  {   
    int distR, distC, distL;
  
    distR = sonarR.ping_cm();                   // get distqnce in cms from the sensor
    if (distR == 0)                                // if too far to measure, return max distance;
      distR = MAX_DISTANCE_CM;  
    sonarDistance_R = sonarRAverage.add(distR);      // add the new value into moving average, use resulting average
    delay(2);
    
    distC = sonarC.ping_cm();                   // get distqnce in cms from the sensor
    if (distC == 0)                                // if too far to measure, return max distance;
      distC = MAX_DISTANCE_CM;  
    sonarDistance_C = sonarCAverage.add(distC);      // add the new value into moving average, use resulting average
    delay(2);
    
    distL = sonarL.ping_cm();                   // get distqnce in cms from the sensor
    if (distL == 0)                                // if too far to measure, return max distance;
      distL = MAX_DISTANCE_CM;  
    sonarDistance_L = sonarLAverage.add(distL);      // add the new value into moving average, use resulting average
    delay(2);
  } // checkSonar()

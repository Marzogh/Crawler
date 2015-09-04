  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  //                                                                                                                                                                                                                                    //
  //                                                                                        Code borrowed from Adafruit's wonderful AHRS Library                                                                                        //
  //                                                                                              https://github.com/adafruit/Adafruit_AHRS                                                                                             //
  //                                                                                                                                                                                                                                    //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  
  
  // Function to configure the sensors on the LSM9DS0 board.
  // You don't need to change anything here, but have the option to select different range and gain values.
  void configureIMU(void)
  {
    // 1.) Set the accelerometer range
    lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G);
    //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_4G);
    //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_6G);
    //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_8G);
    //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_16G);
    
    // 2.) Set the magnetometer sensitivity
    lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS);
    //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_4GAUSS);
    //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_8GAUSS);
    //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_12GAUSS);
  
    // 3.) Setup the gyroscope
    lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS);
    //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_500DPS);
    //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_2000DPS);
    
     // Initialise the LSM9DS0 board.
    if(!lsm.begin())
    {
      #if DEBUG
      // There was a problem detecting the LSM9DS0 ... check your connections
      Serial1.print(F("Ooops, no LSM9DS0 detected ... Check your wiring or I2C ADDR!"));
      #endif
      Serial.print("@#9DS0!");
      while(1);
    }
  }
  
  void readIMU() 
  {
    sensors_vec_t   orientation;
  
    // Use the simple AHRS function to get the current orientation.
    if (ahrs.getOrientation(&orientation))
    {
      /* 'orientation' should have valid .roll and .pitch fields */
      #if DEBUG
      Serial1.print(F("Orientation: "));
      Serial1.print(orientation.roll);
      Serial1.print(F(" "));
      Serial1.print(orientation.pitch);
      Serial1.print(F(" "));
      Serial1.print(orientation.heading);
      Serial1.println(F(""));
      #endif
      
      float heading = (orientation.heading);
    
      // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
      // Find yours here: http://www.magnetic-declination.com/ 
      // Brisbane, QLD, Australia: Magnetic declination: 10° 56' EAST (POSITIVE);  1 degreee = 0.0174532925 radians
      
      #define DEC_ANGLE 1.908
      heading += DEC_ANGLE;
      
      // Correct for when signs are reversed.
      if(heading < 0)
        heading += 2*PI;
        
      // Check for wrap due to addition of declination.
      if(heading > 2*PI)
        heading -= 2*PI;
       
      // Convert radians to degrees for readability.
      float headingDegrees = heading * 180/M_PI; 
      currentHeading = (int)headingDegrees;    // get our current heading
      //return ((int)headingDegrees);
    }
    
    smartDelay(100);
  }

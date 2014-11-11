  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  //                                                                                                                                                                                                                                    //
  //                                                                                    Based on the I2C example code for the Dagu T'Rex Motor controller                                                                               //
  //                                                                                    https://cdn.sparkfun.com/datasheets/Robotics/T_REX_I2C_tester.zip                                                                               //
  //                                                                                                                                                                                                                                    //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  
    
  void MasterReceive()
  {//================================================================= Error Checking ==========================================================
    byte d;
    int i=0;
    Wire.requestFrom(I2Caddress,24);                                // request 24 bytes from device 007
    
    while(Wire.available()<24)                                      // wait for entire data packet to be received
    {
      #if DEBUG
      if(i==0) Serial1.print("Waiting for slave to send data.");     // Only print message once (i==0)
      if(i>0) Serial1.print(".");                                    // print a dot for every loop where buffer<24 bytes
      i++;                                                          // increment i so that message only prints once.
      if(i>79)
      {
        Serial1.println("");
        i=1;
      }
      #endif
    }
    d=Wire.read();                                                  // read start byte from buffer
    if(d!=startbyte)                                                // if start byte not equal to 0x0F                                                    
    {
      #if DEBUG
      Serial1.print(d,DEC);
      #endif
      while(Wire.available()>0)                                     // empty buffer of bad data
      {
        d=Wire.read();
      }
      #if DEBUG
      Serial1.println("  Wrong Start Byte");                        // error message
      #endif
      return;                                                       // quit
    }
    
    //================================================================ Read Data ==============================================================
    Serial.print("@#");
    Serial.print(Wire.read(),DEC);                                  // comment out before debugging
    
    #if DEBUG
    Serial1.print("Slave Error Message:");                          // slave error report
    Serial1.println(Wire.read(),DEC);
    #endif
    
    i=Wire.read()*256+Wire.read();                                  // T'REX battery voltage
    volts=i;
    #if DEBUG
    Serial.print("Battery Voltage:\t");
    Serial.print(int(i/10));Serial.println(".");                      
    Serial.print(i-(int(i/10)*10));Serial.println("V");
    #endif
    
    i=Wire.read()*256+Wire.read();
    lmcur=i;
    #if DEBUG
    Serial.print("Leading  Motor Current:\t");
    Serial.print(i);Serial.println("mA");                           // T'REX left  motor current in mA
    #endif
    
    i=Wire.read()*256+Wire.read();
    lmenc=i;
    #if DEBUG
    Serial.print("Leading  Motor Encoder:\t");
    Serial.println(i);                                              // T'REX left  motor encoder count
    #endif
    
    i=Wire.read()*256+Wire.read();
    rmcur=i;
    #if DEBUG
    Serial.print("Rear Motor Current:\t");
    Serial.print(i);Serial.println("mA");                           // T'REX right motor current in mA
    #endif
    
    i=Wire.read()*256+Wire.read();
    rmenc=i;
    #if DEBUG
    Serial.print("Rear Motor Encoder:\t");
    Serial.println(i);                                              // T'REX right motor encoder count
    #endif
    
    i=Wire.read()*256+Wire.read();
    xaxis=i;
    #if DEBUG
    Serial.print("X-axis:\t\t");
    Serial.println(i);                                              // T'REX X-axis
    #endif
    
    i=Wire.read()*256+Wire.read();
    yaxis=i;
    #if DEBUG
    Serial.print("Y-axis:\t\t");
    Serial.println(i);                                              // T'REX Y-axis
    #endif
    
    i=Wire.read()*256+Wire.read();
    zaxis=i;
    #if DEBUG
    Serial.print("Z-axis:\t\t");
    Serial.println(i);                                              // T'REX Z-axis
    #endif
    
    i=Wire.read()*256+Wire.read();
    deltx=i;
    #if DEBUG
    Serial.print("X-delta:\t\t");
    Serial.println(i);                                              // T'REX X-delta
    #endif
    
    i=Wire.read()*256+Wire.read();
    delty=i;
    #if DEBUG
    Serial.print("Y-delta:\t\t");
    Serial.println(i);                                              // T'REX Y-delta
    #endif
    
    i=Wire.read()*256+Wire.read();
    deltz=i;
    #if DEBUG
    Serial.print("Z-delta:\t\t");
    Serial.println(i);                                              // T'REX Z-delta
    Serial.print("\r\n\n\n");
    #endif
    
  }

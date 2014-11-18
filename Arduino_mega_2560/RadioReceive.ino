  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  //                                                                                                                                                                                                                                    //
  //                                                                                            Functions for Radio Communication and Telemetry                                                                                         //
  //                                                                                                                                                                                                                                    //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

boolean readCommand()
{
  static byte commandIndex=0;
  while(radio.available()>0)
  {
    char incomingbyte = radio.read();
    if (incomingbyte==commandChar)
    {
      commandIndex=0;                                  //Initialize the commandIndex variable
      storeString=true;
    }
    
    if (storeString)
    {
      if (commandIndex==COMMANDSIZE)                    //Check index and abort in case of buffer overflow
      {
        commandIndex=0;
        break;
      }
      
      if (incomingbyte==terminalChar)                    //Data string is complete
      {
        commandRead[commandIndex]=0;                         //Null terminate the C String
        
        storeString=false;
        commandIndex=0;                                  //Reset storeString and commandIndex
        
        return true;                                     //Our data string is complete
      }
      else
      {
        commandRead[commandIndex++]=incomingbyte;
        commandRead[commandIndex]=0;                           //Null terminate the C String
      }
    }
    else
    {
    }
  }
  return false;
}


void parseCommand()
{
  if (readCommand)
    {
      char delimiters[]="$:,";
      char* valPosition;
      
      valPosition=strtok(commandRead, delimiters);
      
      for (int i=0; i<5; i++)
      {
        command[i]=atoi(valPosition);                    // Set to atol for longs, atof for floats
        #if DEBUG
        Serial1.println(command[i]);
        #endif
        valPosition=strtok(NULL, delimiters);
      }
      
    }
    #if DEBUG
      Serial1.print("IO");
      Serial1.println(command[1]);
      Serial1.print("mode");
      Serial1.println(command[2]);
      Serial1.print("Speed");
      Serial1.println(command[3]);
      Serial1.print("Braking");
      Serial1.println(command[4]);
      Serial1.print("Direction");
      Serial1.println(command[5]);
    #endif
}


void waitForRadio()
{
 if (!readCommand)                                    //Wait for radio command
    {
      #if DEBUG
      int w=0;
      
      if(w=0) Serial1.println("Waiting for command");    //Only print message once (w==0)
      if(w>0) Serial1.print(".");                        //Print a dot for every loop where buffer<24 bytes
      i++;                                               //Increment w so that message only prints once.
      if(i>79)
      {
        Serial1.println("");
        i=1;
      }
      #endif
      return;
    } 
}

boolean IO()
{
  if (command[1] == 0)
  return false;
  if (command[1] == 1)
  return true;
}

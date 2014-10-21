void forward()
   {                                    // turns on drive motor in forward and leaves it on
     int s;
     while (s > -1 &&  s < 256)
     {
       s++;
     }
     lmspeed = s;
     rmspeed = s;
     
     for(byte i=0;i<s_no;i++)          // Max value of i should be < total number of servos
    {
      if(sv[i]!=0)                     // If servo is attached
      {
        sv[i]=1500;
      }
    }
    
   }

void backward()
   {                                    // turns on drive motor in reverse and leaves it on
     int s;
     while (s < 1 &&  s > -256)
     {
       s--;
     }
     lmspeed = s;
     rmspeed = s;
     
      for(byte i=0;i<s_no;i++)         // Max value of i should be < total number of steering servos
    {
      if(sv[i]!=0)                     // If servo is attached
      {
        sv[i]=1500;
      }
    }
    
   }

void right()
   {
     int d=25;
     for(byte i=0;i<1;i++)                              // i should be < total number of steering servos
    {
      if(sv[i]>1975) d=0;               // if servo limit is reached, then stop turning servo
      if(sv[i]!=0)                                     // If servo is attached
      {
       sv[i]+=d;                                       // start the turn by the value of int d
       //if(sv[i]>2000) d=0;               // if servo limit is reached, then stop turning servo
      }
    }
   }

void left()
   {
     int d=25;
     for(byte i=0;i<1;i++)                              // i should be < total number of steering servos
    {
      if(sv[i]<1025) d=0;               // if servo limit is reached, then stop turning servo
      if(sv[i]!=0)                                     // If servo is attached
      {
       sv[i]-=d;                                       // start the turn by the value of int d
       //if(sv[i]<1000) d=0;               // if servo limit is reached, then stop turning servo
      }
    }
   }
   
void straight()
{
  for(byte i=0;i<1;i++)                              // i should be < total number of steering servos
    {
      if(sv[i]!=0)                                     // If servo is attached
      {
        sv[i]=1500;
      }
    }
}

void stop()                      //stop both motors
   {
     lmbrake = 1;
     rmbrake = 1;                //Brake both motors
     
     lmspeed = 0;
     rmspeed = 0;                //Stop both motors
     
      for(byte i=0;i<s_no;i++)         // Max value of i should be < total number of steering servos
    {
      if(sv[i]!=0)                     // If servo is attached
      {
        sv[i]=1500;
      }
    }
   }
   
   /* Only needed for skew steering - i.e. with no steering servo
void back_left()
   {
     analogWrite(steerpinB,speed_steer);
     digitalWrite(pinI4,HIGH);          //turn DC Motor B move clockwise
     digitalWrite(pinI3,LOW);
     delay(250);
     backward();
     delay(500);
     stop();
     delay(250);
   }

void back_right()
   {
     analogWrite(steerpinB,speed_steer);
     digitalWrite(pinI4,LOW);          //turn DC Motor B move anticlockwise
     digitalWrite(pinI3,HIGH);
     delay(250);
     backward();
     delay(500);
     stop();
     delay(250);
   } */

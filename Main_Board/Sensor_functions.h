
 void getping_C()//get distance from center PING
   {
      pinMode(pingPin_C, OUTPUT);
      digitalWrite(pingPin_C, LOW);
      delayMicroseconds(2);
      digitalWrite(pingPin_C, HIGH);
      delayMicroseconds(5);
      digitalWrite(pingPin_C, LOW);
      pinMode(echoPin_C, INPUT);
      duration_C = pulseIn(echoPin_C, HIGH);
      cm_C = microsecondsToCM(duration_C);
    }
    
 void getping_R()//get distance from right PING
   {
      pinMode(pingPin_R, OUTPUT);
      digitalWrite(pingPin_R, LOW);
      delayMicroseconds(2);
      digitalWrite(pingPin_R, HIGH);
      delayMicroseconds(5);
      digitalWrite(pingPin_R, LOW);
      pinMode(echoPin_R, INPUT);
      duration_R = pulseIn(echoPin_R, HIGH);
      cm_R = microsecondsToCM(duration_R);
   }
   
 void getping_L()//get distance from left PING
   {
      pinMode(pingPin_L, OUTPUT);
      digitalWrite(pingPin_L, LOW);
      delayMicroseconds(2);
      digitalWrite(pingPin_L, HIGH);
      delayMicroseconds(5);
      digitalWrite(pingPin_L, LOW);
      pinMode(echoPin_L, INPUT);
      duration_L = pulseIn(echoPin_L, HIGH);
      cm_L = microsecondsToCM(duration_L);
   }
   
 void pingAll()
   {
      getping_C();
      delay(2);
      getping_R();
      delay(2);
      getping_L();
      delay(2);
   }
   
 long microsecondsToCM(long microseconds)
  {
    return microseconds / 56;
  }

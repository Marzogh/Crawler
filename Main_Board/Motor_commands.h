
void forward()
   {                                    // turns on drive motor in forward and leaves it on
     analogWrite(speedpinA,speed_drive);//inputs speed_drive value to set the speed
     digitalWrite(pinI2,LOW);          //turn DC Motor A move anticlockwise
     digitalWrite(pinI1,HIGH);
   }

void backward()
   {                                    // turns on drive motor in reverse and leaves it on
     analogWrite(speedpinA,speed_drive);//inputs speed_drive value to set the speed
     digitalWrite(pinI2,HIGH);          //turn DC Motor A move clockwise
     digitalWrite(pinI1,LOW);
   }

void right()
   {
     analogWrite(steerpinB,speed_steer);
     digitalWrite(pinI4,HIGH);          //turn DC Motor B move clockwise
     digitalWrite(pinI3,LOW);
     forward();
     delay(300);
     analogWrite(steerpinB,LOW);
   }

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

void left()
   {
     analogWrite(steerpinB,speed_steer);
     digitalWrite(pinI4,LOW);          //turn DC Motor B move anticlockwise
     digitalWrite(pinI3,HIGH);
     forward();
     delay(300);
     analogWrite(steerpinB,LOW);
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
   }

void stop()                      //stop both motors
   {
     digitalWrite(speedpinA,LOW);// Turn off power to the pin to stop the motor. 
     digitalWrite(steerpinB,LOW);// Turn off power to the pin to stop the motor.
   }

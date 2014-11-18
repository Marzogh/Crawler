/*
//  Debouncing Function
//Pass it the previous button state and get back the current debounced button state.
// All variables declared withing this function are local variables - i.e. used only within the function

boolean debounce(boolean past)
{
  boolean present = digitalRead(BUTTON);                //read button state
  if (past != present)                                  //if its different
  {
    delay(5);                                           //wait 5ms
    present = digitalRead(BUTTON);                      //and read it again

  return present;                                       //return current value
  }
}
*/
void readButtons()
{
  RTH = digitalRead (left_button);
  RC = digitalRead (right_button);
  power = digitalRead (up_button);
  Auto = digitalRead (down_button);
  
  setHome = digitalRead (stick_button);
  steering = analogRead (level_stick); //0-1023 - Right to left
  speed = analogRead (vertical_stick); //0-1023 - Forward to Reverse
}


void powerCheck()
{
   if (power == HIGH && previousPower == LOW && millis() - time > DEBOUNCE)
  {
    IO = 1;
    time=millis();
    previousPower = power;
  }

    if (power == HIGH && previousPower == HIGH && millis() - time > DEBOUNCE)
  {
    IO = 0;
    time=millis();
    previousPower = LOW;
  }
  
    if (power == LOW && previousPower == LOW && millis() - time > DEBOUNCE)
  {
    IO = 0;
    time=millis();
  }
  
    if (power == LOW && previousPower == HIGH && millis() - time > DEBOUNCE)
  {
    IO = 1;
    time=millis();
  }
}

void modeCheck()
{
   if (Auto = HIGH && millis() - time > DEBOUNCE)          // Auto mode
   {
     mode = 0;
     previousAuto == HIGH;
     previousRTH, previousRC == LOW;
   }
   if (RTH = HIGH && millis() - time > DEBOUNCE)          // RTH mode
   {
     mode = 1;
     previousRTH == HIGH;
     previousRC, previousAuto == LOW;
   }
   if (RC = HIGH && millis() - time > DEBOUNCE)          // RC mode
   {
     mode = 2;
     previousRC == HIGH;
     previousRTH, previousAuto == LOW;
   }
}

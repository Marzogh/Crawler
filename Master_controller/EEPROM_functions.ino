void getAddresses()
{
  currentQuestAddress  = EEPROM.getAddress(sizeof(byte));
  homeAddress.latitude = EEPROM.getAddress(sizeof(double));
  homeAddress.longitude = EEPROM.getAddress(sizeof(double));
  Address1.latitude = EEPROM.getAddress(sizeof(double));
  Address1.longitude = EEPROM.getAddress(sizeof(double));
  Address2.latitude = EEPROM.getAddress(sizeof(double));
  Address2.longitude = EEPROM.getAddress(sizeof(double));
  Address3.latitude = EEPROM.getAddress(sizeof(double));
  Address3.longitude = EEPROM.getAddress(sizeof(double));
  Address4.latitude = EEPROM.getAddress(sizeof(double));
  Address4.longitude = EEPROM.getAddress(sizeof(double));
  Address5.latitude = EEPROM.getAddress(sizeof(double));
  Address5.longitude = EEPROM.getAddress(sizeof(double));

  #ifdef SERIALECHO
  Serial.println(currentQuestAddress);
  Serial.println(homeAddress.latitude);
  Serial.println(homeAddress.longitude);
  Serial.println(Address1.latitude);
  Serial.println(Address1.longitude);
  Serial.println(Address2.latitude);
  Serial.println(Address2.longitude);
  Serial.println(Address3.latitude);
  Serial.println(Address3.longitude);
  Serial.println(Address4.latitude);
  Serial.println(Address4.longitude);
  Serial.println(Address5.latitude);
  Serial.println(Address5.longitude);
  #endif
}

void updateTarget()
{
  switch (currentQuestAddress) {
    case 0:
    target.latitude = Home.latitude;
    target.longitude = Home.longitude;
    break;

    case 1:
    target.latitude = quest1.latitude;
    target.longitude = quest1.longitude;
    break;

    case 2:
    target.latitude = quest2.latitude;
    target.longitude = quest2.longitude;
    break;

    case 3:
    target.latitude = quest3.latitude;
    target.longitude = quest3.longitude;
    break;

    case 4:
    target.latitude = quest4.latitude;
    target.longitude = quest4.longitude;
    break;

    case 5:
    target.latitude = quest5.latitude;
    target.longitude = quest5.longitude;
    break;
  }
}


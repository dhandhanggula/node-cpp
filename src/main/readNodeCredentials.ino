//====================================================================
//====================================================================
//==This file used to read ID and network properties saved in EEPROM==
//====================================================================
//====================================================================

void readNodeCredentials()
{
  // Read node ID in EEPROM
  for(int i=0; i<10; i++)
  {
    nodeID += (char)EEPROM.read(i);
  }
}

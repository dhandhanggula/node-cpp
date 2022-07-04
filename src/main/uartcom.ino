void uartcom(String serialcommand)
{
  serialcommand.replace("\n", "");
  serialcommand.replace("\r", "");

  if(serialcommand == ""){return;}

  else
  {
    String command = parsing(serialcommand, '|', 0);

    // PING
    if(command == "ping")
    {
      String nodeTarget = parsing(serialcommand, '|', 1);

      if(lastDestination == nodeTarget){
        ping(nodeTarget, lastRoute);
        return;
      }

      else
      {
        // del cache
        lastDestination = "";
        lastRoute = "";

        //do rreq
        String response = routeDiscovery(nodeTarget);

        if(response == "201")
        {
          //ping
          ping(nodeTarget, lastRoute);
          return;
        }

        Serial.print(F("Cant connect to "));
        Serial.println(nodeTarget);
        return;
      }
    }
    

    // SET ID
    if(command == "set_id")
    {
      String newID = parsing(serialcommand, '|', 1);

      if(newID.length() != 6) 
      {
        Serial.println(F("Error. ID length must be 6 characters"));
        return; 
      }

      for(int i=0; i<6; i++)
      {
        EEPROM.write(i, 0);
        delay(1);
        EEPROM.write(i, newID.charAt(i));
        delay(1);
      }

      Serial.println(F("Device ID has been changed. Please restart your device!"));
      return;
    }

    // READ ID
    if(command == "read_id")
    {
      Serial.println(nodeID);
      return;
    }

    // READ NETWORK TIME
    if(command == "read_network_time")
    {
      Serial.println(rtc.now().unixtime(), HEX);
      return;
    }

    // AT Command, just to check device is connected
    if(command == "at")
    {
      Serial.println("OK");
      return;
    }
    
    // RREQ --> ex: rreq|YrCGaw
    if(command == "rreq")
    {
      String nodeTarget = parsing(serialcommand, '|', 1);
      routeDiscovery(nodeTarget);
      return;
    }

    // print route --> ex: print_route
    if(command == "print_route")
    {
      Serial.print(lastDestination);
      Serial.print(F("--"));
      Serial.println(lastRoute);
      return;
    }

    // delete command --> ex: del_route
    if(command == "del_route")
    {
      lastDestination = "";
      lastRoute = "";
      Serial.println(F("Route cache deleted"));
      return;
    }
    
  }
}

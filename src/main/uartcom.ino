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
      for(int i=0; i<20; i++)
      {
        if(String(destinationBook[i]) == parsing(serialcommand, '|', 1))
        {
          ping(destinationBook[i], routeBook[i]);
          return;
        }
      }

      Serial.println("Error. Code : 404");
      Serial.print("Didn't have link with ");
      Serial.println(nodeTarget);

      Serial.print("Please do RREQ with rreq|{destination node}");
      return;
    }
    

    // SET ID
    if(command == "set_id")
    {
      String newID = parsing(serialcommand, '|', 1);

      if(newID.length() != 6) 
      {
        Serial.println("Error. ID length must be 6 characters");
        return; 
      }
      else
      {
        for(int i=0; i<EEPROM.length(); i++)
        {
          EEPROM.write(i, 0);
        }
        for(int i=0; i<6; i++)
        {
          EEPROM.write(i, newID.charAt(i));
        }

        delay(100);
        //update nodeID
        readNodeCredentials();
        Serial.print("New ID :");
        Serial.println(nodeID);
        return;
      }
    }

    // READ ID
    if(command == "read_id")
    {
      //readNodeCredentials();
      Serial.println(nodeID);
      return;
    }

    // READ NETWORK TIME
    if(command == "read_network_time")
    {
      Serial.println(rtc.now().unixtime(), HEX);
      return;
    }

    // AT Command
    if(command == "at")
    {
      Serial.println("OK");
      return;
    }
    
    // RREQ
    if(command == "rreq")
    {
      String nodeTarget = parsing(serialcommand, '|', 1);
      routeDiscovery(nodeTarget);
      return;
    }

    // Print Routing Cache
    if(command == "print_route_cache")
    {
      Serial.print("No. \t");
      Serial.print("Destination \t");
      Serial.println("Route Path");

      for(int i=0; i<sizeof(destinationBook); i++)
      {
        Serial.print(i);
        Serial.print("\t");
        Serial.print(destinationBook[i]);
        Serial.print("\t \t");
        Serial.println(routeBook[i]);
      }
    }
    
  }
}
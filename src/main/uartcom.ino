void uartcom(String serialcommand)
{
  serialcommand.replace("\n", "");
  serialcommand.replace("\r", "");

  if(serialcommand == ""){return;}

  else
  {
    String command = parsing(serialcommand, '|', 0);

    // AT Command, just to check device is connected
    if(command == "at")
    {
      Serial.println(F("OK"));
      return;
    }
    
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
          Serial.println(F("Please wait a second before send ping message."));
          return;
        }

        Serial.print(F("Cant connect to "));
        Serial.println(nodeTarget);
      }
      return;
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

    // SET AES KEY
    if(command == "set_key")
    {
      String aes_key = parsing(serialcommand, '|', 1);

      if(aes_key.length() != 16)
      {
        Serial.println(F("Error! Key must be 16 character"));
        return;
      }

      // save to EEPROM
      for(int i=0; i<16; i++)
      {
        EEPROM.write(7 + i, 0);
        delay(1);
        EEPROM.write(7 + i, aes_key.charAt(i));
        delay(1);
      }

      // Calculate key expansion
      Serial.println("Please wait! Generate key expansion");

      // Do expansion key first and save in byte array
      // save w[0], w[1], w[2], and w[3] first
      byte w[44][4] PROGMEM = {{}};                 // to save expansion key w calculation

      Serial.print("EEPROM ADDRESS Start from 7 until ");
      w[0][0] = (int)EEPROM.read(7);
      w[0][1] = (int)EEPROM.read(8);
      w[0][2] = (int)EEPROM.read(9);
      w[0][3] = (int)EEPROM.read(10);

      w[1][0] = (int)EEPROM.read(11);
      w[1][1] = (int)EEPROM.read(12);
      w[1][2] = (int)EEPROM.read(13);
      w[1][3] = (int)EEPROM.read(14);

      w[2][0] = (int)EEPROM.read(15);
      w[2][1] = (int)EEPROM.read(16);
      w[2][2] = (int)EEPROM.read(17);
      w[2][3] = (int)EEPROM.read(18);

      w[3][0] = (int)EEPROM.read(19);
      w[3][1] = (int)EEPROM.read(20);
      w[3][2] = (int)EEPROM.read(21);
      w[3][3] = (int)EEPROM.read(22);

      // doing calculation for w[4] until w[43]
      for(int i=4; i<44; i++)
      {
        // save w[i-1] ke temp
        byte temp_key[4];

        for(int j=0; j<4; j++)
        {
          temp_key[j] = w[i-1][j]; 
        }

        // lakukan fungsi g jika kelipatan 4
        if(i % 4 == 0)
        {
          // geser 1 byte ke kiri
          byte temp = temp_key[0];
          temp_key[0] = temp_key[1];
          temp_key[1] = temp_key[2];
          temp_key[2] = temp_key[3];
          temp_key[3] = temp;

          // substitusi ke s-box
          for(int j=0; j<4; j++){
            temp_key[j] = s[temp_key[j]];
          }

          
          // XOR dengan matrix RC, 00, 00, 00
          if(i == 4){temp_key[0] = temp_key[0] ^ 0x01;}
          if(i == 8){temp_key[0] = temp_key[0] ^ 0x02;}
          if(i == 12){temp_key[0] = temp_key[0] ^ 0x04;}
          if(i == 16){temp_key[0] = temp_key[0] ^ 0x08;}
          if(i == 20){temp_key[0] = temp_key[0] ^ 0x10;}
          if(i == 24){temp_key[0] = temp_key[0] ^ 0x20;}
          if(i == 28){temp_key[0] = temp_key[0] ^ 0x40;}
          if(i == 32){temp_key[0] = temp_key[0] ^ 0x80;}
          if(i == 36){temp_key[0] = temp_key[0] ^ 0x1b;}
          if(i == 40){temp_key[0] = temp_key[0] ^ 0x36;}

          // XOR
          for(int j=0; j<4; j++){
            w[i][j] = w[i-4][j] ^ temp_key[j];
          }
        }

        if(i % 4 != 0){
          // XOR only
          for(int j=0; j<4; j++)
          {
            w[i][j] = w[i-1][j] ^ w[i-4][j];
          }
        }
      }

      // Save key Expansion to EEPROM
      for(int x=0; x<44; x++)
      {
        EEPROM.write(23 + (x*4) + 0, 0);
        delay(1);
        EEPROM.write(23 + (x*4) + 0, w[x][0]);
        delay(1);
        EEPROM.write(23 + (x*4) + 1, 0);
        delay(1);
        EEPROM.write(23 + (x*4) + 1, w[x][1]);
        delay(1);
        EEPROM.write(23 + (x*4) + 2, 0);
        delay(1);
        EEPROM.write(23 + (x*4) + 2, w[x][2]);
        delay(1);
        EEPROM.write(23 + (x*4) + 3, 0);
        delay(1);
        EEPROM.write(23 + (x*4) + 3, w[x][3]);
        delay(1);

        if(x==43)
        {
          Serial.println(23 + (x*4) + 3);
        }
      }

      Serial.println(F("AES Encryption key has been changed. Please restart your device!"));
      return;
    }
    

    // Read AES Key
    if(command == "read_key")
    {
      String aes_key;
      for(int i=0; i<16; i++){
        aes_key += (char)EEPROM.read(7 + i);
      }
      Serial.println(aes_key);
      return;
    }
    
    // RREQ --> ex: rreq|YrCGaw
    if(command == "rreq")
    {
      String nodeTarget = parsing(serialcommand, '|', 1);
      routeDiscovery(nodeTarget);
      return;
    }

    if(command == "msg")
    {
      meshMsg(parsing(serialcommand, '|', 1), parsing(serialcommand, '|', 2), parsing(serialcommand, '|', 3));
      return;
    }


    if(command == "encrypt")
    {

      String inputMessage = parsing(serialcommand, '|', 1);
      encryptAES(inputMessage);
      // test decrypt
      //aes_decrypt(encryptAES(inputMessage));
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

    if(command == "msgid_his")
    {
      for(int i=0; i<10; i++)
      {
        Serial.println(msgIDHistory[i]);
      }
      return;
    }
  }
}

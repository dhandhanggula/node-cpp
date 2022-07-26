String aes_encrypt(String plaintext)
{ 
  
  byte inputState[16] = {};               // to save plaintext in byte
  byte tempState[16] = {};                // temporary state

  unsigned char chipertext_char[24] = {}; // save base64
  String chipertext;                      // chipertext in string

  // Convert plaintext and aes_key in byte
  for(int i=0; i<16; i++)
  {
    inputState[i] = (int)plaintext[i];
  }

  // Encrypt here
  // First round
  for(int i=0; i<16; i++){
    internalState[i] = inputState[i] ^ (int)EEPROM.read(7+i);
  }
  
  // 2nd until 9th round
  for(int round = 1; round<10; round++)
  {
    // SUBBYTES
    for(int i=0; i<16; i++){
      internalState[i] = s[internalState[i]];
    }

    // SHIFTROWS
    for(int i=0; i<16; i++){
      tempState[i] = internalState[i];
    }

    internalState[0] = tempState[0];
    internalState[1] = tempState[5];
    internalState[2] = tempState[10];
    internalState[3] = tempState[15];
    internalState[4] = tempState[4];
    internalState[5] = tempState[9];
    internalState[6] = tempState[14];
    internalState[7] = tempState[3];
    internalState[8] = tempState[8];
    internalState[9] = tempState[13];
    internalState[10] = tempState[2];
    internalState[11] = tempState[7];
    internalState[12] = tempState[12];
    internalState[13] = tempState[1];
    internalState[14] = tempState[6];
    internalState[15] = tempState[11];

    // MIXCOLUMNS
    for(int i=0; i<16; i++){
      tempState[i] = internalState[i];
    }

    for(int i=0; i<4; i++)
    {
      internalState[i*4 + 0] = gmul(tempState[i*4], 2) ^ gmul(tempState[i*4+1], 3) ^ tempState[i*4+2] ^ tempState[i*4+3];
      internalState[i*4 + 1] = tempState[i*4] ^ gmul(tempState[i*4+1], 2) ^ gmul(tempState[i*4+2], 3) ^ tempState[i*4+3];
      internalState[i*4 + 2] = tempState[i*4] ^ tempState[i*4+1] ^ gmul(tempState[i*4+2], 2) ^ gmul(tempState[i*4+3], 3);
      internalState[i*4 + 3] = gmul(tempState[i*4], 3) ^ tempState[i*4+1] ^ tempState[i*4+2] ^ gmul(tempState[i*4+3], 2);
    }

    // ADDROUNDKEY
    for(int i=0; i<4; i++)
    {
      // Start fom EEPROM 23
      internalState[(i*4) + 0] = internalState[(i*4) + 0] ^ (int)EEPROM.read(round*16 + (i*4) + 23 + 0);
      internalState[(i*4) + 1] = internalState[(i*4) + 1] ^ (int)EEPROM.read(round*16 + (i*4) + 23 + 1);
      internalState[(i*4) + 2] = internalState[(i*4) + 2] ^ (int)EEPROM.read(round*16 + (i*4) + 23 + 2);
      internalState[(i*4) + 3] = internalState[(i*4) + 3] ^ (int)EEPROM.read(round*16 + (i*4) + 23 + 3);
    }

  }
  
  // 10th round
  // SUBBYTES
  for(int i=0; i<16; i++){
    internalState[i] = s[internalState[i]];
  }

  // SHIFTROWS
  for(int i=0; i<16; i++){
    tempState[i] = internalState[i];
  }

  internalState[0] = tempState[0];
  internalState[1] = tempState[5];
  internalState[2] = tempState[10];
  internalState[3] = tempState[15];
  internalState[4] = tempState[4];
  internalState[5] = tempState[9];
  internalState[6] = tempState[14];
  internalState[7] = tempState[3];
  internalState[8] = tempState[8];
  internalState[9] = tempState[13];
  internalState[10] = tempState[2];
  internalState[11] = tempState[7];
  internalState[12] = tempState[12];
  internalState[13] = tempState[1];
  internalState[14] = tempState[6];
  internalState[15] = tempState[11];


  // ADDROUNDKEY
  // EEPROM start from 183
  for(int i=0; i<4; i++)
  {
  internalState[(i*4) + 0] = internalState[(i*4) + 0] ^ (int)EEPROM.read(183 + (i*4));
  internalState[(i*4) + 1] = internalState[(i*4) + 1] ^ (int)EEPROM.read(184 + (i*4));
  internalState[(i*4) + 2] = internalState[(i*4) + 2] ^ (int)EEPROM.read(185 + (i*4));
  internalState[(i*4) + 3] = internalState[(i*4) + 3] ^ (int)EEPROM.read(186 + (i*4));
  }


  encode_base64(internalState, 16, chipertext_char);


  for(int i=0; i<24; i++)
  {
    //Serial.println(internalState[i], HEX);
    chipertext += (char)chipertext_char[i];
  }

  return chipertext;
}


///////////////////////////////////////////////////////////////////////////

byte gmul(byte a, int b){
    byte tmp = (a << 1) & 0xff;

    if (b == 2){
      if(a<128)
      {
        return tmp;
      }
      else{
        return tmp ^ 0x1b;
      }
    }

    if (b == 3){
      if(a<128)
      {
        return tmp ^ a;
      }
      else{
        return tmp ^ 0x1b ^a;
      }
    }
}


String encryptAES(String inputPlaintext)
{
  String chipertext_encrypt = "";
  int plaintextLength = inputPlaintext.length();

  if(plaintextLength < 16)
  {
    for(int i=0; i<16-plaintextLength; i++){
      inputPlaintext += "=";
    }
  }

  if(plaintextLength > 16)
  {
    int plaintextMod = plaintextLength % 16;
    if(plaintextMod != 0)
    {
      for(int i=0; i<16-plaintextMod; i++){
        inputPlaintext += "=";
      }
    }
  }

  //parse for 16 byte
  plaintextLength = inputPlaintext.length();
  int aesDivide = plaintextLength / 16;

  for(int y=0; y<aesDivide; y++)
  {
    String tempString = "";

    for(int z=0; z<16; z++)
    {
      tempString += inputPlaintext[y*16 + z];
    }
    chipertext_encrypt += aes_encrypt(tempString);
  }

  Serial.println(chipertext_encrypt);
  return chipertext_encrypt;
}

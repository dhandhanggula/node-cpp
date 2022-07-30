String aes_encrypt(String plaintext)
{
  byte w[44][4] = {{}};               // to save expansion key w calculation

  byte tempState[16] = {};

  unsigned char chipertext_char[22] = {};
  String chipertext;

  // save to w[][]
  for(int x=0; x<44; x++)
  {
    w[x][0] = (int)EEPROM.read(23 + (x*4) + 0);
    w[x][1] = (int)EEPROM.read(23 + (x*4) + 1);
    w[x][2] = (int)EEPROM.read(23 + (x*4) + 2);
    w[x][3] = (int)EEPROM.read(23 + (x*4) + 3);
  }

  // Encrypt here
  // First round
  for(int i=0; i<16; i++){
    internalState[i] = (int)plaintext[i] ^ ((int)EEPROM.read(7+i));
  }
  
  // 2nd until 9th round
  for(int round = 1; round<10; round++)
  {

    // SUBBYTES
    for(int i=0; i<16; i++){
      internalState[i] = sbox(internalState[i]);
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
      internalState[i*4 + 0] = gauss(0x02, tempState[i*4]) ^ gauss(0x03, tempState[i*4+1]) ^ tempState[i*4+2] ^ tempState[i*4+3];
      internalState[i*4 + 1] = tempState[i*4] ^ gauss(0x02, tempState[i*4+1]) ^ gauss(0x03, tempState[i*4+2]) ^ tempState[i*4+3];
      internalState[i*4 + 2] = tempState[i*4] ^ tempState[i*4+1] ^ gauss(0x02, tempState[i*4+2]) ^ gauss(0x03, tempState[i*4+3]);
      internalState[i*4 + 3] = gauss(0x03, tempState[i*4]) ^ tempState[i*4+1] ^ tempState[i*4+2] ^ gauss(0x02, tempState[i*4+3]);
    }

    // ADDROUNDKEY
    for(int i=0; i<4; i++)
    {
    internalState[(i*4) + 0] = internalState[(i*4) + 0] ^ w[round*4 + i][0];
    internalState[(i*4) + 1] = internalState[(i*4) + 1] ^ w[round*4 + i][1];
    internalState[(i*4) + 2] = internalState[(i*4) + 2] ^ w[round*4 + i][2];
    internalState[(i*4) + 3] = internalState[(i*4) + 3] ^ w[round*4 + i][3];
    }
  }
  
  // 10th round
  // SUBBYTES
  for(int i=0; i<16; i++){
    internalState[i] = sbox(internalState[i]);
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
  for(int i=0; i<4; i++)
  {
  internalState[(i*4) + 0] = internalState[(i*4) + 0] ^ w[40 + i][0];
  internalState[(i*4) + 1] = internalState[(i*4) + 1] ^ w[40 + i][1];
  internalState[(i*4) + 2] = internalState[(i*4) + 2] ^ w[40 + i][2];
  internalState[(i*4) + 3] = internalState[(i*4) + 3] ^ w[40 + i][3];
  }
  
  encode_base64(internalState, 22, chipertext_char);


  for(int i=0; i<22; i++)
  {
    chipertext += (char)chipertext_char[i];
  }

  return chipertext;
}


///////////////////////////////////////////////////////////////////////////


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

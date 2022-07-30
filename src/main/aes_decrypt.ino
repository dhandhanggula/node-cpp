
String aes_decrypt(String chipertext_to_decrypt)
{
  byte w[44][4] = {{}};               // to save expansion key w calculation

  byte tempState[16] = {};
  byte inputBase64[22] = {};

  unsigned char plaintext_char[16] = {};
  String plaintext;

  byte keys[16];

  for(int i=0; i<16; i++)
  {
    keys[i] = (int)EEPROM.read(i+7);
  }

  // save to w[][]
  for(int x=0; x<44; x++)
  {
    w[x][0] = (int)EEPROM.read(23 + (x*4) + 0);
    w[x][1] = (int)EEPROM.read(23 + (x*4) + 1);
    w[x][2] = (int)EEPROM.read(23 + (x*4) + 2);
    w[x][3] = (int)EEPROM.read(23 + (x*4) + 3);
  }

  // Convert chipertext in array
  for(int i=0; i<22; i++)
  {
    inputBase64[i] = (int)chipertext_to_decrypt[i];
  }

  // convert base64 to hex
  decode_base64(inputBase64, internalState);

  // Start decrypt here
  // 10th round

  // INV-ADDROUNDKEY
  for(int i=0; i<4; i++)
  {
    internalState[(i*4) + 0] = internalState[(i*4) + 0] ^ w[40 + i][0];
    internalState[(i*4) + 1] = internalState[(i*4) + 1] ^ w[40 + i][1];
    internalState[(i*4) + 2] = internalState[(i*4) + 2] ^ w[40 + i][2];
    internalState[(i*4) + 3] = internalState[(i*4) + 3] ^ w[40 + i][3];
  }

  // INV-SHIFTROWS
  for(int i=0; i<16; i++){
    tempState[i] = internalState[i];
  }

  internalState[0] = tempState[0];
  internalState[1] = tempState[13];
  internalState[2] = tempState[10];
  internalState[3] = tempState[7];
  internalState[4] = tempState[4];
  internalState[5] = tempState[1];
  internalState[6] = tempState[14];
  internalState[7] = tempState[11];
  internalState[8] = tempState[8];
  internalState[9] = tempState[5];
  internalState[10] = tempState[2];
  internalState[11] = tempState[15];
  internalState[12] = tempState[12];
  internalState[13] = tempState[9];
  internalState[14] = tempState[6];
  internalState[15] = tempState[3];



  // INV-SUBBYTES
  for(int i=0; i<16; i++){
    internalState[i] = d_sbox(internalState[i]);
  }

  // for round 9 until 2
  for(int round = 0; round<9; round++)
  {
    // INV-ADDROUNDKEY
    for(int i=0; i<4; i++)
    {
      internalState[(i*4) + 0] = internalState[(i*4) + 0] ^ w[36-(4*round) + i][0];
      internalState[(i*4) + 1] = internalState[(i*4) + 1] ^ w[36-(4*round) + i][1];
      internalState[(i*4) + 2] = internalState[(i*4) + 2] ^ w[36-(4*round) + i][2];
      internalState[(i*4) + 3] = internalState[(i*4) + 3] ^ w[36-(4*round) + i][3];
    }

    // INV - MIXCOLUMN
    for(int i=0; i<16; i++){
      tempState[i] = internalState[i];
    }

    for(int i=0; i<4; i++)
    {
      internalState[i*4 + 0] = gauss(0x0e, tempState[i*4]) ^ gauss(0x0b, tempState[i*4+1]) ^ gauss(0x0d, tempState[i*4+2]) ^ gauss(0x09, tempState[i*4+3]);
      internalState[i*4 + 1] = gauss(0x09, tempState[i*4]) ^ gauss(0x0e, tempState[i*4+1]) ^ gauss(0x0b, tempState[i*4+2]) ^ gauss(0x0d, tempState[i*4+3]);
      internalState[i*4 + 2] = gauss(0x0d, tempState[i*4]) ^ gauss(0x09, tempState[i*4+1]) ^ gauss(0x0e, tempState[i*4+2]) ^ gauss(0x0b, tempState[i*4+3]);
      internalState[i*4 + 3] = gauss(0x0b, tempState[i*4]) ^ gauss(0x0d, tempState[i*4+1]) ^ gauss(0x09, tempState[i*4+2]) ^ gauss(0x0e, tempState[i*4+3]);
    }

    // INV-SHIFTROWS
    for(int i=0; i<16; i++){
      tempState[i] = internalState[i];
    }

    internalState[0] = tempState[0];
    internalState[1] = tempState[13];
    internalState[2] = tempState[10];
    internalState[3] = tempState[7];
    internalState[4] = tempState[4];
    internalState[5] = tempState[1];
    internalState[6] = tempState[14];
    internalState[7] = tempState[11];
    internalState[8] = tempState[8];
    internalState[9] = tempState[5];
    internalState[10] = tempState[2];
    internalState[11] = tempState[15];
    internalState[12] = tempState[12];
    internalState[13] = tempState[9];
    internalState[14] = tempState[6];
    internalState[15] = tempState[3];
    
    // INV-SUBBYTES
    for(int i=0; i<16; i++){
      internalState[i] = d_sbox(internalState[i]);
    }
  }

  // First round
  for(int i=0; i<16; i++){
    internalState[i] = internalState[i] ^ keys[i];
    plaintext += (char)internalState[i];
  }

  return plaintext;
}


String decryptAES(String inputChipertext)
{
  String plaintext_decrypt = "";
  int chipertextLength = inputChipertext.length();  

  if(chipertextLength % 22 != 0)
  {
    Serial.println(F("Chipertext length is not valid."));
    return inputChipertext;
  }

  // parse if more than 22 byte
  int aesDivide = chipertextLength / 22;
  for(int y=0; y<aesDivide; y++)
  {
    String tempString = "";

    for(int z=0; z<22; z++)
    {
      tempString += inputChipertext[y*22 + z];
    }

    plaintext_decrypt += aes_decrypt(tempString);
  }

  Serial.println(plaintext_decrypt);
  return plaintext_decrypt;
}


byte d_sbox(byte stateHex)
{
  return EEPROM.read((int)stateHex + 256);
}




byte gauss(byte first, byte second)
{
  if(first == 0x00 || second == 0x00)
  {
    return 0x00;
  }

  // L table
  int sum = (int)EEPROM.read((int)first + 512) + (int)EEPROM.read((int)second + 512);
  if(sum > 255)
  {
    sum = sum - 255;
  }

  // E table
  byte gauss_value = EEPROM.read(sum + 768);

  return gauss_value;
}
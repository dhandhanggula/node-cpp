String aes_encrypt(String plaintext)
{
  byte w[44][4] = {{}};               // to save expansion key w calculation
  byte inputState[16] = {};           // to save plaintext in byte

  //byte internalState[16] = {};        // to save state in process
  byte tempState[16] = {};

  unsigned char chipertext_char[16] = {};
  String chipertext;

  // Convert plaintext and aes_key in byte
  for(int i=0; i<16; i++)
  {
    inputState[i] = (int)plaintext[i];
  }

  // Do expansion key first and save in byte array
  // save w[0], w[1], w[2], and w[3] first
  w[0][0] = keys[0];
  w[0][1] = keys[1];
  w[0][2] = keys[2];
  w[0][3] = keys[3];

  w[1][0] = keys[4];
  w[1][1] = keys[5];
  w[1][2] = keys[6];
  w[1][3] = keys[7];

  w[2][0] = keys[8];
  w[2][1] = keys[9];
  w[2][2] = keys[10];
  w[2][3] = keys[11];

  w[3][0] = keys[12];
  w[3][1] = keys[13];
  w[3][2] = keys[14];
  w[3][3] = keys[15];

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
        temp_key[j] = sbox(temp_key[j]);
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



  // Encrypt here
  // First round
  for(int i=0; i<16; i++){
    internalState[i] = inputState[i] ^ keys[i];
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
      internalState[i*4 + 0] = gmul(tempState[i*4], 2) ^ gmul(tempState[i*4+1], 3) ^ tempState[i*4+2] ^ tempState[i*4+3];
      internalState[i*4 + 1] = tempState[i*4] ^ gmul(tempState[i*4+1], 2) ^ gmul(tempState[i*4+2], 3) ^ tempState[i*4+3];
      internalState[i*4 + 2] = tempState[i*4] ^ tempState[i*4+1] ^ gmul(tempState[i*4+2], 2) ^ gmul(tempState[i*4+3], 3);
      internalState[i*4 + 3] = gmul(tempState[i*4], 3) ^ tempState[i*4+1] ^ tempState[i*4+2] ^ gmul(tempState[i*4+3], 2);
    }

    // ADDROUNDKEY
    internalState[0] = internalState[0] ^ w[round*4 + 0][0];
    internalState[1] = internalState[1] ^ w[round*4 + 0][1];
    internalState[2] = internalState[2] ^ w[round*4 + 0][2];
    internalState[3] = internalState[3] ^ w[round*4 + 0][3];

    internalState[4] = internalState[4] ^ w[round*4 + 1][0];
    internalState[5] = internalState[5] ^ w[round*4 + 1][1];
    internalState[6] = internalState[6] ^ w[round*4 + 1][2];
    internalState[7] = internalState[7] ^ w[round*4 + 1][3];

    internalState[8] = internalState[8] ^ w[round*4 + 2][0];
    internalState[9] = internalState[9] ^ w[round*4 + 2][1];
    internalState[10] = internalState[10] ^ w[round*4 + 2][2];
    internalState[11] = internalState[11] ^ w[round*4 + 2][3];

    internalState[12] = internalState[12] ^ w[round*4 + 3][0];
    internalState[13] = internalState[13] ^ w[round*4 + 3][1];
    internalState[14] = internalState[14] ^ w[round*4 + 3][2];
    internalState[15] = internalState[15] ^ w[round*4 + 3][3];
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
  internalState[0] = internalState[0] ^ w[40][0];
  internalState[1] = internalState[1] ^ w[40][1];
  internalState[2] = internalState[2] ^ w[40][2];
  internalState[3] = internalState[3] ^ w[40][3];

  internalState[4] = internalState[4] ^ w[41][0];
  internalState[5] = internalState[5] ^ w[41][1];
  internalState[6] = internalState[6] ^ w[41][2];
  internalState[7] = internalState[7] ^ w[41][3];

  internalState[8] = internalState[8] ^ w[42][0];
  internalState[9] = internalState[9] ^ w[42][1];
  internalState[10] = internalState[10] ^ w[42][2];
  internalState[11] = internalState[11] ^ w[42][3];

  internalState[12] = internalState[12] ^ w[43][0];
  internalState[13] = internalState[13] ^ w[43][1];
  internalState[14] = internalState[14] ^ w[43][2];
  internalState[15] = internalState[15] ^ w[43][3];

  encode_base64(internalState, 16, chipertext_char);
  chipertext = chipertext + (char)chipertext_char[0];
  chipertext = chipertext + (char)chipertext_char[1];
  chipertext = chipertext + (char)chipertext_char[2];
  chipertext = chipertext + (char)chipertext_char[3];
  chipertext = chipertext + (char)chipertext_char[4];
  chipertext = chipertext + (char)chipertext_char[5];
  chipertext = chipertext + (char)chipertext_char[6];
  chipertext = chipertext + (char)chipertext_char[7];
  chipertext = chipertext + (char)chipertext_char[8];
  chipertext = chipertext + (char)chipertext_char[9];
  chipertext = chipertext + (char)chipertext_char[10];
  chipertext = chipertext + (char)chipertext_char[11];
  chipertext = chipertext + (char)chipertext_char[12];
  chipertext = chipertext + (char)chipertext_char[13];
  chipertext = chipertext + (char)chipertext_char[14];
  chipertext = chipertext + (char)chipertext_char[15];

  Serial.println(chipertext);

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


byte sbox(byte stateHex)
{
  if(stateHex == 0x00){return 0x63;}
  if(stateHex == 0x01){return 0x7c;}
  if(stateHex == 0x02){return 0x77;}
  if(stateHex == 0x03){return 0x7b;}
  if(stateHex == 0x04){return 0xf2;}
  if(stateHex == 0x05){return 0x6b;}
  if(stateHex == 0x06){return 0x6f;}
  if(stateHex == 0x07){return 0xc5;}
  if(stateHex == 0x08){return 0x30;}
  if(stateHex == 0x09){return 0x01;}
  if(stateHex == 0x0a){return 0x67;}
  if(stateHex == 0x0b){return 0x2b;}
  if(stateHex == 0x0c){return 0xfe;}
  if(stateHex == 0x0d){return 0xd7;}
  if(stateHex == 0x0e){return 0xab;}
  if(stateHex == 0x0f){return 0x76;}

  if(stateHex == 0x10){return 0xca;}
  if(stateHex == 0x11){return 0x82;}
  if(stateHex == 0x12){return 0xc9;}
  if(stateHex == 0x13){return 0x7d;}
  if(stateHex == 0x14){return 0xfa;}
  if(stateHex == 0x15){return 0x59;}
  if(stateHex == 0x16){return 0x47;}
  if(stateHex == 0x17){return 0xf0;}
  if(stateHex == 0x18){return 0xad;}
  if(stateHex == 0x19){return 0xd4;}
  if(stateHex == 0x1a){return 0xa2;}
  if(stateHex == 0x1b){return 0xaf;}
  if(stateHex == 0x1c){return 0x9c;}
  if(stateHex == 0x1d){return 0xa4;}
  if(stateHex == 0x1e){return 0x72;}
  if(stateHex == 0x1f){return 0xc0;}

  if(stateHex == 0x20){return 0xb7;}
  if(stateHex == 0x21){return 0xfd;}
  if(stateHex == 0x22){return 0x93;}
  if(stateHex == 0x23){return 0x26;}
  if(stateHex == 0x24){return 0x36;}
  if(stateHex == 0x25){return 0x3f;}
  if(stateHex == 0x26){return 0xf7;}
  if(stateHex == 0x27){return 0xcc;}
  if(stateHex == 0x28){return 0x34;}
  if(stateHex == 0x29){return 0xa5;}
  if(stateHex == 0x2a){return 0xe5;}
  if(stateHex == 0x2b){return 0xf1;}
  if(stateHex == 0x2c){return 0x71;}
  if(stateHex == 0x2d){return 0xd8;}
  if(stateHex == 0x2e){return 0x31;}
  if(stateHex == 0x2f){return 0x15;}

  if(stateHex == 0x30){return 0x04;}
  if(stateHex == 0x31){return 0xc7;}
  if(stateHex == 0x32){return 0x23;}
  if(stateHex == 0x33){return 0xc3;}
  if(stateHex == 0x34){return 0x18;}
  if(stateHex == 0x35){return 0x96;}
  if(stateHex == 0x36){return 0x05;}
  if(stateHex == 0x37){return 0x9a;}
  if(stateHex == 0x38){return 0x07;}
  if(stateHex == 0x39){return 0x12;}
  if(stateHex == 0x3a){return 0x80;}
  if(stateHex == 0x3b){return 0xe2;}
  if(stateHex == 0x3c){return 0xeb;}
  if(stateHex == 0x3d){return 0x27;}
  if(stateHex == 0x3e){return 0xb2;}
  if(stateHex == 0x3f){return 0x75;}

  if(stateHex == 0x40){return 0x09;}
  if(stateHex == 0x41){return 0x83;}
  if(stateHex == 0x42){return 0x2c;}
  if(stateHex == 0x43){return 0x1a;}
  if(stateHex == 0x44){return 0x1b;}
  if(stateHex == 0x45){return 0x6e;}
  if(stateHex == 0x46){return 0x5a;}
  if(stateHex == 0x47){return 0xa0;}
  if(stateHex == 0x48){return 0x52;}
  if(stateHex == 0x49){return 0x3b;}
  if(stateHex == 0x4a){return 0xd6;}
  if(stateHex == 0x4b){return 0xb3;}
  if(stateHex == 0x4c){return 0x29;}
  if(stateHex == 0x4d){return 0xe3;}
  if(stateHex == 0x4e){return 0x2f;}
  if(stateHex == 0x4f){return 0x84;}

  if(stateHex == 0x50){return 0x53;}
  if(stateHex == 0x51){return 0xd1;}
  if(stateHex == 0x52){return 0x00;}
  if(stateHex == 0x53){return 0xed;}
  if(stateHex == 0x54){return 0x20;}
  if(stateHex == 0x55){return 0xfc;}
  if(stateHex == 0x56){return 0xb1;}
  if(stateHex == 0x57){return 0x5b;}
  if(stateHex == 0x58){return 0x6a;}
  if(stateHex == 0x59){return 0xcb;}
  if(stateHex == 0x5a){return 0xbe;}
  if(stateHex == 0x5b){return 0x39;}
  if(stateHex == 0x5c){return 0x4a;}
  if(stateHex == 0x5d){return 0x4c;}
  if(stateHex == 0x5e){return 0x58;}
  if(stateHex == 0x5f){return 0xcf;}

  if(stateHex == 0x60){return 0xd0;}
  if(stateHex == 0x61){return 0xef;}
  if(stateHex == 0x62){return 0xaa;}
  if(stateHex == 0x63){return 0xfb;}
  if(stateHex == 0x64){return 0x43;}
  if(stateHex == 0x65){return 0x4d;}
  if(stateHex == 0x66){return 0x33;}
  if(stateHex == 0x67){return 0x85;}
  if(stateHex == 0x68){return 0x45;}
  if(stateHex == 0x69){return 0xf9;}
  if(stateHex == 0x6a){return 0x02;}
  if(stateHex == 0x6b){return 0x7f;}
  if(stateHex == 0x6c){return 0x50;}
  if(stateHex == 0x6d){return 0x3c;}
  if(stateHex == 0x6e){return 0x9f;}
  if(stateHex == 0x6f){return 0xa8;}

  if(stateHex == 0x70){return 0x51;}
  if(stateHex == 0x71){return 0xa3;}
  if(stateHex == 0x72){return 0x40;}
  if(stateHex == 0x73){return 0x8f;}
  if(stateHex == 0x74){return 0x92;}
  if(stateHex == 0x75){return 0x9d;}
  if(stateHex == 0x76){return 0x38;}
  if(stateHex == 0x77){return 0xf5;}
  if(stateHex == 0x78){return 0xbc;}
  if(stateHex == 0x79){return 0xb6;}
  if(stateHex == 0x7a){return 0xda;}
  if(stateHex == 0x7b){return 0x21;}
  if(stateHex == 0x7c){return 0x10;}
  if(stateHex == 0x7d){return 0xff;}
  if(stateHex == 0x7e){return 0xf3;}
  if(stateHex == 0x7f){return 0xd2;}

  if(stateHex == 0x80){return 0xcd;}
  if(stateHex == 0x81){return 0x0c;}
  if(stateHex == 0x82){return 0x13;}
  if(stateHex == 0x83){return 0xec;}
  if(stateHex == 0x84){return 0x5f;}
  if(stateHex == 0x85){return 0x97;}
  if(stateHex == 0x86){return 0x44;}
  if(stateHex == 0x87){return 0x17;}
  if(stateHex == 0x88){return 0xc4;}
  if(stateHex == 0x89){return 0xa7;}
  if(stateHex == 0x8a){return 0x7e;}
  if(stateHex == 0x8b){return 0x3d;}
  if(stateHex == 0x8c){return 0x64;}
  if(stateHex == 0x8d){return 0x5d;}
  if(stateHex == 0x8e){return 0x19;}
  if(stateHex == 0x8f){return 0x73;}

  if(stateHex == 0x90){return 0x60;}
  if(stateHex == 0x91){return 0x81;}
  if(stateHex == 0x92){return 0x4f;}
  if(stateHex == 0x93){return 0xdc;}
  if(stateHex == 0x94){return 0x22;}
  if(stateHex == 0x95){return 0x2a;}
  if(stateHex == 0x96){return 0x90;}
  if(stateHex == 0x97){return 0x88;}
  if(stateHex == 0x98){return 0x46;}
  if(stateHex == 0x99){return 0xee;}
  if(stateHex == 0x9a){return 0xb8;}
  if(stateHex == 0x9b){return 0x14;}
  if(stateHex == 0x9c){return 0xde;}
  if(stateHex == 0x9d){return 0x5e;}
  if(stateHex == 0x9e){return 0x0b;}
  if(stateHex == 0x9f){return 0xdb;}

  if(stateHex == 0xa0){return 0xe0;}
  if(stateHex == 0xa1){return 0x32;}
  if(stateHex == 0xa2){return 0x3a;}
  if(stateHex == 0xa3){return 0x0a;}
  if(stateHex == 0xa4){return 0x49;}
  if(stateHex == 0xa5){return 0x06;}
  if(stateHex == 0xa6){return 0x24;}
  if(stateHex == 0xa7){return 0x5c;}
  if(stateHex == 0xa8){return 0xc2;}
  if(stateHex == 0xa9){return 0xd3;}
  if(stateHex == 0xaa){return 0xac;}
  if(stateHex == 0xab){return 0x62;}
  if(stateHex == 0xac){return 0x91;}
  if(stateHex == 0xad){return 0x95;}
  if(stateHex == 0xae){return 0xe4;}
  if(stateHex == 0xaf){return 0x79;}

  if(stateHex == 0xb0){return 0xe7;}
  if(stateHex == 0xb1){return 0xc8;}
  if(stateHex == 0xb2){return 0x37;}
  if(stateHex == 0xb3){return 0x6d;}
  if(stateHex == 0xb4){return 0x8d;}
  if(stateHex == 0xb5){return 0xd5;}
  if(stateHex == 0xb6){return 0x4e;}
  if(stateHex == 0xb7){return 0xa9;}
  if(stateHex == 0xb8){return 0x6c;}
  if(stateHex == 0xb9){return 0x56;}
  if(stateHex == 0xba){return 0xf4;}
  if(stateHex == 0xbb){return 0xea;}
  if(stateHex == 0xbc){return 0x65;}
  if(stateHex == 0xbd){return 0x7a;}
  if(stateHex == 0xbe){return 0xae;}
  if(stateHex == 0xbf){return 0x08;}

  if(stateHex == 0xc0){return 0xba;}
  if(stateHex == 0xc1){return 0x78;}
  if(stateHex == 0xc2){return 0x25;}
  if(stateHex == 0xc3){return 0x2e;}
  if(stateHex == 0xc4){return 0x1c;}
  if(stateHex == 0xc5){return 0xa6;}
  if(stateHex == 0xc6){return 0xb4;}
  if(stateHex == 0xc7){return 0xc6;}
  if(stateHex == 0xc8){return 0xe8;}
  if(stateHex == 0xc9){return 0xdd;}
  if(stateHex == 0xca){return 0x74;}
  if(stateHex == 0xcb){return 0x1f;}
  if(stateHex == 0xcc){return 0x4b;}
  if(stateHex == 0xcd){return 0xbd;}
  if(stateHex == 0xce){return 0x8b;}
  if(stateHex == 0xcf){return 0x8a;}

  if(stateHex == 0xd0){return 0x70;}
  if(stateHex == 0xd1){return 0x3e;}
  if(stateHex == 0xd2){return 0xb5;}
  if(stateHex == 0xd3){return 0x66;}
  if(stateHex == 0xd4){return 0x48;}
  if(stateHex == 0xd5){return 0x03;}
  if(stateHex == 0xd6){return 0xf6;}
  if(stateHex == 0xd7){return 0x0e;}
  if(stateHex == 0xd8){return 0x61;}
  if(stateHex == 0xd9){return 0x35;}
  if(stateHex == 0xda){return 0x57;}
  if(stateHex == 0xdb){return 0xb9;}
  if(stateHex == 0xdc){return 0x86;}
  if(stateHex == 0xdd){return 0xc1;}
  if(stateHex == 0xde){return 0x1d;}
  if(stateHex == 0xdf){return 0x9e;}

  if(stateHex == 0xe0){return 0xe1;}
  if(stateHex == 0xe1){return 0xf8;}
  if(stateHex == 0xe2){return 0x98;}
  if(stateHex == 0xe3){return 0x11;}
  if(stateHex == 0xe4){return 0x69;}
  if(stateHex == 0xe5){return 0xd9;}
  if(stateHex == 0xe6){return 0x8e;}
  if(stateHex == 0xe7){return 0x94;}
  if(stateHex == 0xe8){return 0x9b;}
  if(stateHex == 0xe9){return 0x1e;}
  if(stateHex == 0xea){return 0x87;}
  if(stateHex == 0xeb){return 0xe9;}
  if(stateHex == 0xec){return 0xce;}
  if(stateHex == 0xed){return 0x55;}
  if(stateHex == 0xee){return 0x28;}
  if(stateHex == 0xef){return 0xdf;}

  if(stateHex == 0xf0){return 0x8c;}
  if(stateHex == 0xf1){return 0xa1;}
  if(stateHex == 0xf2){return 0x89;}
  if(stateHex == 0xf3){return 0x0d;}
  if(stateHex == 0xf4){return 0xbf;}
  if(stateHex == 0xf5){return 0xe6;}
  if(stateHex == 0xf6){return 0x42;}
  if(stateHex == 0xf7){return 0x68;}
  if(stateHex == 0xf8){return 0x41;}
  if(stateHex == 0xf9){return 0x99;}
  if(stateHex == 0xfa){return 0x2d;}
  if(stateHex == 0xfb){return 0x0f;}
  if(stateHex == 0xfc){return 0xb0;}
  if(stateHex == 0xfd){return 0x54;}
  if(stateHex == 0xfe){return 0xbb;}
  if(stateHex == 0xff){return 0x16;}
}
//====================================================================
//====================================================================
// Setup node initialization --> used in void setup()
//
// What it set :
// - Read node ID in EEPROM
// - Initialize serial via Serial.begin()
// - Initialize LoRa module
//====================================================================
//====================================================================

void setupNode()
{
  // Reading the credentials saved in EEPROM
  readNodeCredentials();

  Serial.begin(9600);

  // Activate LoRa
  Serial.println("========================================");
  Serial.print("Node with ID ");
  Serial.print(nodeID);
  Serial.println(" is active");
  Serial.println("========================================");

  if (!LoRa.begin(networkFreq)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial.print("Frequency : ");
  Serial.println(networkFreq);

  LoRa.setTxPower(txPower);
  Serial.print("Transmitter power (dB) : ");
  Serial.println(txPower);

  LoRa.setSpreadingFactor(spreadingFactor);
  Serial.print("Spreading Factor : ");
  Serial.println(spreadingFactor);

  LoRa.setSignalBandwidth(signalBandwidth);
  Serial.print("Signal Bandwidth : ");
  Serial.println(signalBandwidth);

  LoRa.setSyncWord(networkSync);
  Serial.print("Network Sync Word : ");
  Serial.println(networkSync, HEX);

  Serial.println("Node is activated successfully =========");
  Serial.println("========================================");
  Serial.println(" ");
}
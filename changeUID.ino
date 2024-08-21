#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9    // Reset pin
#define SS_PIN 10    // Slave select pin

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

// Define the new UID you want to write to the card (replace "..." with desired bytes)
byte newUID[] = {0x..., 0x..., 0x..., 0x...}; // New UID (4 bytes)

void setup() {
  Serial.begin(9600); // Initialize serial communications
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card

  Serial.println("Place your card to the reader...");
}

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) return;

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) return;

  Serial.print("Current UID: ");
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();

  // Halt PICC
  mfrc522.PICC_HaltA();

  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();

  // Write new UID to the card (only works on writable UID cards)
  if (writeNewUID(newUID)) {
    Serial.println("UID changed successfully.");
  } else {
    Serial.println("Failed to change UID.");
  }

  delay(1000); // Add a delay before the next loop iteration
}

bool writeNewUID(byte *newUID) {
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF; // Default key: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF

  // Authenticate using the default key on block 0
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 0x00, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("PCD_Authenticate() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }

  // Prepare the data to be written to block 0
  byte block0[16] = {newUID[0], newUID[1], newUID[2], newUID[3], 0x08, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  status = mfrc522.MIFARE_Write(0, block0, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("MIFARE_Write() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }

  return true;
}

void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
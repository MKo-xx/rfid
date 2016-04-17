/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example to Read NTag21x tag data with read/fast read commands
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * This sample shows how to read NTag21x tag data with read/fast read commands
 * NOTE: for more informations read the README.rst
 * 
 * @author Mkrtich Soghomonyan
 * @license Released into the public domain.
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);      // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  mfrc522.PCD_SetAntennaGain(MFRC522::RxGain_max);
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(300);
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(300);
    return;
  }

  // read from page 10 - OK
  {
    byte buffer[10];
    byte bufferSize = 10;
    MFRC522::StatusCode result_status;
    
    result_status = mfrc522.NTAG_21x_FastRead(10, 10, buffer, &bufferSize);
    Serial.print("NTAG_21x_FastRead result = ");
    Serial.println(mfrc522.GetStatusCodeName(result_status));
  
    if (MFRC522::STATUS_OK == result_status) {
      Serial.println(bufferSize);
      for (byte i=0; i < bufferSize; i++) {
        Serial.print(buffer[i], HEX);
        Serial.print(" ");
      }
      Serial.println("");
    } else {
      return;
    }
  }

  // add password protection - OK
  {
    byte password[] = {0x12, 0x34, 0x56, 0x78};
    byte pack[] = {0x9A, 0xBC};
    MFRC522::StatusCode result_status;
    
    result_status = mfrc522.NTAG_21x_AddPasswordProtect(password, pack, MFRC522::NTAG_213);
    Serial.print("NTAG_21x_PasswordProtect result = ");
    Serial.println(mfrc522.GetStatusCodeName(result_status));
    if (MFRC522::STATUS_OK != result_status) {
      return;
    }
  }

  // write to page 10 - ERROR no auth
  {
    byte buffer[] = {0x11, 0x45, 0x82, 0x17};
    MFRC522::StatusCode result_status;
    
    result_status = mfrc522.NTAG_21x_Write(10, buffer);
    Serial.print("NTAG_21x_Write result = ");
    Serial.println(mfrc522.GetStatusCodeName(result_status));
  }

  // auth
  {
    byte password[] = {0x12, 0x34, 0x56, 0x78};
    byte pack[2];
    MFRC522::StatusCode result_status;

    result_status = mfrc522.NTAG_21x_PasswordAuth(password, pack);
    Serial.print("NTAG_21x_PasswordAuth result = ");
    Serial.println(mfrc522.GetStatusCodeName(result_status));

    if (MFRC522::STATUS_OK == result_status) {
      Serial.print(pack[0], HEX);
      Serial.println(pack[1], HEX);
    } else {
      return;
    }
    
  }

  // write to page 10 - OK
  {
    byte buffer[] = {0x10, 0x45, 0x82, 0x17};
    MFRC522::StatusCode result_status;
    
    result_status = mfrc522.NTAG_21x_Write(10, buffer);
    Serial.print("NTAG_21x_Write result = ");
    Serial.println(mfrc522.GetStatusCodeName(result_status));
  }

  // read from page 10 - OK
  {
    byte buffer[10];
    byte bufferSize = 10;
    MFRC522::StatusCode result_status;
    
    result_status = mfrc522.NTAG_21x_FastRead(10, 10, buffer, &bufferSize);
    Serial.print("NTAG_21x_FastRead result = ");
    Serial.println(mfrc522.GetStatusCodeName(result_status));
  
    if (MFRC522::STATUS_OK == result_status) {
      Serial.println(bufferSize);
      for (byte i=0; i < bufferSize; i++) {
        Serial.print(buffer[i], HEX);
        Serial.print(" ");
      }
      Serial.println("");
    } else {
      return;
    }
  }

  // remove auth - OK
  {
    MFRC522::StatusCode result_status;
    
    result_status = mfrc522.NTAG_21x_RemovePasswordProtect(MFRC522::NTAG_213);
    Serial.print("NTAG_21x_RemovePasswordProtect result = ");
    Serial.println(mfrc522.GetStatusCodeName(result_status));
  }

  //
  delay(5000);
}



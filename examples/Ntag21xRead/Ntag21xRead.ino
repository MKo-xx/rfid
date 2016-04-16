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

  //
  {
    byte buffer[100];
    byte bufferSize = 100;
    MFRC522::StatusCode result_status;
  
    //
    result_status = mfrc522.NTAG_21x_Read(0, buffer, &bufferSize);
    Serial.print("NTAG_21x_FastRead result = ");
    Serial.println(result_status);
  
    Serial.println(bufferSize);
    for (byte i=0; i < bufferSize; i++) {
      Serial.print(buffer[i], HEX);
      Serial.print(" ");
    }
    Serial.println("");
  }

  //
  {
    byte buffer[100];
    byte bufferSize = 100;
    MFRC522::StatusCode result_status;
    
    result_status = mfrc522.NTAG_21x_FastRead(0, 10, buffer, &bufferSize);
    Serial.print("NTAG_21x_FastRead result = ");
    Serial.println(result_status);
  
    Serial.println(bufferSize);
    for (byte i=0; i < bufferSize; i++) {
      Serial.print(buffer[i], HEX);
      Serial.print(" ");
    }
    Serial.println("");
  }

  //
  delay(5000);
}


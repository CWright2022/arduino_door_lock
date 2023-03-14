/*
Code for Arduino hotel door model
for RITSEC Imagine 2023 exhibition
by Cayden Wright
*/
#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include <NfcAdapter.h>
PN532_SPI interface(SPI, 10); // create a PN532 SPI interface with the SPI CS terminal located at digital pin 10
NfcAdapter nfc = NfcAdapter(interface); // create an NFC adapter object
String tagId = "None";
// byte nuidPICC[4];

int redLed = 9;
int greenLed = 8;

void setup(void) {
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  Serial.begin(9600);
  nfc.begin();
}

void loop() {
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, LOW);
  readNFC();
  delay(500);
}

void readNFC() {
  // Serial.println("run nfc function");
  if (nfc.tagPresent()) {
    // Serial.println("tag is present");
    NfcTag tag = nfc.read();
    if (tag.hasNdefMessage()) {
      // Serial.println("tag has NDEF record");
      NdefMessage message = tag.getNdefMessage();
      int recordCount = message.getRecordCount();
      message.print();
      for (int i = 0; i < recordCount; i++) {
        NdefRecord record = message.getRecord(i);
        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);
        String payloadString = "";
        for (int c = 0; c < payloadLength; c++) {
          payloadString += (char)payload[c];
        }
        Serial.println(payloadString);
        payloadString.remove(0,3);
        Serial.println(payloadString);
        if (payloadString == "unlockdoor") {
          Serial.println("unlock!");
          digitalWrite(redLed, LOW);
          digitalWrite(greenLed, HIGH);
          delay(2000);
        }
        payloadString = "";
      }
    }
  }
}
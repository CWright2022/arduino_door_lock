/*
Code for Arduino hotel door model
for RITSEC Imagine 2023 exhibition
by Cayden Wright
*/
#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include <NfcAdapter.h>
PN532_SPI interface(SPI, 10);            // create a PN532 SPI interface with the SPI CS terminal located at digital pin 10
NfcAdapter nfc = NfcAdapter(interface);  // create an NFC adapter object
String tagId = "None";
// byte nuidPICC[4];

int redLed = 5;
int greenLed = 4;
int readyLed = 3;
int buzzer = 6;

void doubleBeep(unsigned int freq) {
  tone(buzzer, freq);
  delay(75);
  noTone(buzzer);
  delay(75);
  tone(buzzer, freq);
  delay(75);
  noTone(buzzer);
}

void setup(void) {
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(readyLed, OUTPUT);
  Serial.begin(9600);
  nfc.begin();
  tone(buzzer, 775);
  delay(500);
  noTone(buzzer);
  digitalWrite(buzzer, LOW);
  digitalWrite(readyLed, HIGH);
}

void loop() {
  // Serial.println("run nfc function");
  if (nfc.tagPresent()) {
    // Serial.println("tag is present");
    digitalWrite(readyLed, LOW);
    NfcTag tag = nfc.read();
    if (tag.hasNdefMessage()) {
      // Serial.println("tag has NDEF record");
      NdefMessage message = tag.getNdefMessage();
      int recordCount = message.getRecordCount();
      message.print();
      for (int i = 0; i < recordCount; i++) {
        // Serial.print("record number");
        // Serial.println(i);
        NdefRecord record = message.getRecord(i);
        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);
        String payloadString = "";
        for (int c = 0; c < payloadLength; c++) {
          payloadString += (char)payload[c];
        }
        // Serial.println(payloadString);
        payloadString.remove(0, 3);
        // Serial.println(payloadString);
        if (payloadString == "unlockdoor") {
          Serial.println("unlock!");
          digitalWrite(greenLed, HIGH);
          doubleBeep(440);
          delay(1775);
          digitalWrite(greenLed, LOW);
        } else {
          Serial.println("incorrect");
          digitalWrite(redLed, HIGH);
          doubleBeep(131);
          delay(1775);
          digitalWrite(redLed, LOW);
        }
        payloadString = "";
        digitalWrite(readyLed, HIGH);
      }
    }
  }
  // delay(500);
}
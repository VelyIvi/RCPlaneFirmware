#include <Arduino.h>
#include <SPI.h>                      //the communication interface with the modem
#include "RF24.h"                     //the library which helps us to control the radio modem

#define CE_PIN  22
#define CSN_PIN 21
// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);

uint8_t address[][6] = { "1Node", "2Node" };
bool radioNumber = 0;
bool role = false;  // true = TX node, false = RX node

int msg[32];


void setup(){
  Serial.begin(115200);
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }

  Serial.println(F("Radio started"));
  
  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.

  radio.openWritingPipe(address[radioNumber]);  // always uses pipe 0
  radio.openReadingPipe(1, address[!radioNumber]);  // using pipe 1

  if (role) {
    radio.stopListening();  // put radio in TX mode
  } else {
    radio.startListening();  // put radio in RX mode
  }
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
}

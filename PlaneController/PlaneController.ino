
#include <Arduino.h>
#include <SPI.h>                      //the communication interface with the modem
#include "RF24.h"                     //the library which helps us to control the radio modem

#define CE_PIN  22
#define CSN_PIN 21
// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);

uint8_t address[][6] = { "1Node", "2Node" };
bool radioNumber = 0;
bool role = true;  // true = TX node, false = RX node


int msg[32];
int sendDelay = 50;


void setup(void) {
  Serial.begin(115200);
  while (!Serial);
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (!radio.begin()) {
      Serial.print(".");
      delay(100);
    }
  } else { 
    Serial.println(F("Radio started"));
  }

  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.

  radio.openWritingPipe(address[radioNumber]);  // always uses pipe 0
  radio.openReadingPipe(1, address[!radioNumber]);  // using pipe 1

  if (role) {
    radio.stopListening();  // put radio in TX mode
  } else {
    radio.startListening();  // put radio in RX mode
  }


}

struct ControllerData
{
  int leftVertical = 0;
  int leftHorizontal = 0;
  int rightVertical = 0;
  int rightHorizontal = 0;
};

ControllerData controllerData;

void loop(void) {
  controllerData.leftVertical =  map(analogRead(34), 0, 4095, 0, 180);
  controllerData.leftHorizontal = map(analogRead(35), 0, 4095, 0, 180);
  bool report = radio.write(&controllerData, sizeof(controllerData));
  if (report) {
    Serial.println(F("Transmission successful! "));  // payload was delivered
    Serial.print(controllerData.leftVertical);
    Serial.print("   ");
    Serial.println(controllerData.leftHorizontal);
    delay(sendDelay);
  } else {
    Serial.println(F("Transmission failed or timed out"));  // payload was not delivered
  }
}

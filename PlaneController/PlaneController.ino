#include <SPI.h>                      //the communication interface with the modem
#include "RF24.h"                     //the library which helps us to control the radio modem

#define CE_PIN  22
#define CSN_PIN 21
// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);

uint8_t address[][6] = { "1Node", "2Node" };
bool radioNumber = 0;
bool role = true;  // true = TX node, false = RX node


int msg[2];
int sendDelay = 50;


void setup(void) {
  Serial.begin(115200);
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }

  //  radio.begin();                      //it activates the modem.
  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.

  radio.openWritingPipe(address[radioNumber]);  // always uses pipe 0
  radio.openReadingPipe(1, address[!radioNumber]);  // using pipe 1

  if (role) {
    radio.stopListening();  // put radio in TX mode
  } else {
    radio.startListening();  // put radio in RX mode
  }


}

void loop(void) {
  msg[0] =  map(analogRead(34), 0, 4095, 0, 180);
  msg[1] = map(analogRead(35), 0, 4095, 0, 180);
  bool report = radio.write(msg, sizeof(msg));
  if (report) {
    Serial.println(F("Transmission successful! "));  // payload was delivered
    Serial.print(msg[0]);
    Serial.print("   ");
    Serial.println(msg[1]);
    delay(sendDelay);
  } else {
    Serial.println(F("Transmission failed or timed out"));  // payload was not delivered
  }
}

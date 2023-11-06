#include <ESP32Servo.h>
#include <SPI.h>      //the communication interface with the modem
#include "RF24.h"     //the library which helps us to control the radio modem

Servo myServo;        //define the servo name
Servo myServo1;

#define CE_PIN  22
#define CSN_PIN 21
// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);

uint8_t address[][6] = { "1Node", "2Node" };
bool radioNumber = 1;
bool role = false;  // true = TX node, false = RX node


int msg[2];

void setup(){
  myServo.attach(17);                //3 is a digital pin to which servo signal connected.
  myServo1.attach(16);                //3 is a digital pin to which servo signal connected.


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

void loop(){
  uint8_t pipe;
  if (radio.available(&pipe)) {            //checks whether any data have arrived at the address of the modem
    bool done = false;              //returns a “true” value if we received some data, or “false” if no data.
    radio.read(&msg, 2);
    Serial.print(msg[0]);
    Serial.print("  ");
    Serial.println(msg[1]);

    myServo.write(msg[0]);
    myServo1.write(msg[1]);

    
  }
}

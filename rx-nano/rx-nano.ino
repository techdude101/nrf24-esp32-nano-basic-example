/** @file rx-nano.ino
 * 
 * @brief Program to receive data from another device. Received data is printed to the console.
 *
 * @author Mark McKee
 */ 

#include <SPI.h>
#include "RF24.h"

#define BAUD_RATE 115200

#define NRF24_CE 7
#define NRF24_CSN 8

#define RADIO_ADDRESS_ESP32 0
#define RADIO_ADDRESS_NANO 1

RF24 radio(NRF24_CE, NRF24_CSN);

uint8_t addresses[][6] = { "ESP32", "Nano" };

void setup() {
  Serial.begin(BAUD_RATE);

  // Wait until serial port is available
  while (!Serial) {
    delay(1);
  }

  Serial.println("Initialising radio ... ");
  
  // Initialise the transceiver on the SPI bus
  while (!radio.begin()) {
    Serial.println("Radio hardware is not responding!");
    delay(1000);
  }

  Serial.println("Entering RX mode ... ");
  // Put the radio in RX mode
  radio.openReadingPipe(0, addresses[RADIO_ADDRESS_ESP32]);
  radio.startListening();
}

void loop() {
  uint8_t pipe_number;
  if (radio.available()) {
    uint8_t payload_size_in_bytes = radio.getPayloadSize();
    uint8_t payload[payload_size_in_bytes];
    
    radio.read(&payload, payload_size_in_bytes);
    Serial.print(F("Received "));
    Serial.print(payload_size_in_bytes);
    Serial.print(F(" bytes on pipe "));
    Serial.print(pipe_number);
    Serial.print(F(": "));
    Serial.println((char*)payload);
  }
}

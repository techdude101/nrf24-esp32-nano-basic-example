/** @file tx-esp32.ino
 * 
 * @brief On power on, test transmission of data to check hardware is working and wiring is correct.
 *
 * @author Mark McKee
 */ 

#include <SPI.h>
#include "RF24.h"

#define BAUD_RATE 115200

#define NRF24_CE 16
#define NRF24_CSN 17

#define RADIO_ADDRESS_INDEX_ESP32 0
#define RADIO_ADDRESS_INDEX_NANO 1

RF24 radio(NRF24_CE, NRF24_CSN);

uint8_t addresses[][6] = { "ESP32", "Nano" };

void setup() {
  const char test_data[] = "abc123";
  
  Serial.begin(BAUD_RATE);
  
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }

  SPI.begin();

  Serial.println("Initialising radio ... ");
  // initialize the transceiver on the SPI bus
  while (!radio.begin()) {
    Serial.println("Radio hardware is not responding!");
    delay(1000);
  }

  Serial.println("Putting radio in TX mode");
  // Put radio in TX mode
  radio.stopListening();
  radio.openWritingPipe(addresses[RADIO_ADDRESS_INDEX_ESP32]);

  Serial.printf("About to transmit data: %s", test_data);

  unsigned long tx_start_time = micros();
  radio.write(&test_data, sizeof(test_data));
  unsigned long tx_end_time = micros();

  Serial.printf("Transmit time = %ld μs", (tx_end_time - tx_start_time));
}

void loop() {

}

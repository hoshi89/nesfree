#include <VirtualWire.h>

#define  rx_pin     12
#define  latch_pin   2
#define  clock_pin   3
#define  data_pin    4

uint8_t  data = 0xff;
uint8_t  data_latched = 0xff;

void setup() {

  // Attach interrupt for when the NES is latching the current state
  attachInterrupt(digitalPinToInterrupt(latch_pin), provideState, RISING);
  
  // Attach another interrupt for when the NES is clocking data
  attachInterrupt(digitalPinToInterrupt(clock_pin), updateClock, RISING);
}

void loop() {
  // Listen to transmitted data

  // Store current input state

}

void provideState() {

  // The latch is HIGH, copy the current state
  // and present first bit on the data line
  data_latched = data;
  if((data_latched & 1) > 0) {
    digitalWrite(data_pin, HIGH);
  } else {
    digitalWrite(data_pin, LOW);
  }

  // Wait for latch to go LOW again
  while(digitalRead(latch_pin) == HIGH);
}

void updateClock() {

  // When clock has gone HIGH, shift data
  // and present first bit on data line
  data_latched = data_latched >> 1;
  if((data_latched & 1) > 0) {
    digitalWrite(data_pin, HIGH);
  } else {
    digitalWrite(data_pin, LOW);
  }

  // Wait for clock to go LOW again
  while(digitalRead(clock_pin) == HIGH);
}

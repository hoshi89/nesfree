#include <VirtualWire.h>

// NOTE: Only pin 2 and 3 works with interrupts
#define  latch_pin   2
#define  clock_pin   3
#define  data_pin    4
#define  rx_pin      5

uint8_t  data = 0xff;
uint8_t  latched_data = 0xff;

void setup() {

  pinMode(latch_pin, INPUT);
  pinMode(clock_pin, INPUT);
  pinMode(data_pin, OUTPUT);

  vw_set_rx_pin(rx_pin);
  vw_setup(2000);
  vw_rx_start();

  attachInterrupt(digitalPinToInterrupt(latch_pin), latchState, RISING);
  attachInterrupt(digitalPinToInterrupt(clock_pin), shiftData, RISING);
}

void loop() {
  
  uint8_t buf[1];
  uint8_t buflen = 1;
  
  if (vw_get_message(buf, &buflen)) {
    data = buf[0];
  }
}

void latchState() {
  latched_data = data;
  if((latched_data & 1) > 0) {
    digitalWrite(data_pin, HIGH);
  } else {
    digitalWrite(data_pin, LOW);
  }
}

void shiftData() {
  latched_data = latched_data >> 1;
  if((latched_data & 1) > 0) {
    digitalWrite(data_pin, HIGH);
  } else {
    digitalWrite(data_pin, LOW);
  }
}


#include <VirtualWire.h>

#define  tx_pin     12
#define  latch_pin   2
#define  clock_pin   3
#define  data_pin    4

void setup() {

  // Set up the pin modes
  pinMode(tx_pin, OUTPUT);
  pinMode(latch_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  pinMode(data_pin, INPUT);

  // Initial states
  digitalWrite(latch_pin, LOW);
  digitalWrite(clock_pin, LOW);

  // Initialize VirtualWire
  vw_set_tx_pin(tx_pin);
  vw_setup(2000);
}

void loop() {
  
  // Read current controller state
  uint8_t state = getCurrentState();

  // Send the byte representing the input state
  vw_send(&state, 1);

  // Wait for byte to be sent before continuing
  vw_wait_tx();
}

uint8_t getCurrentState() {
  
  // Prepare output
  uint8_t state = 0x00;
  
  // Latch the state
  digitalWrite(latch_pin, HIGH);
  digitalWrite(latch_pin, LOW);

  for(int i = 0; i < 8; i++) {

    // Read the state of the current pin
    if(digitalRead(data_pin) == HIGH) {
      state |= (1 << i);
    }

    // Clock cycle
    digitalWrite(clock_pin, HIGH);
    digitalWrite(clock_pin, LOW);
    
  }
  
  return state;
}


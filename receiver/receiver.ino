#include <VirtualWire.h>

#define  rx_pin      MOSI // 11
#define  latch_pin   SS   // 10
#define  clock_pin   SCK  // 13
#define  data_pin    MISO // 12

uint8_t  data = 0xff;

void setup() {

  // turn on SPI in slave mode
  SPCR |= bit (SPE);

  // have to send on master in, *slave out*
  pinMode(data_pin, OUTPUT);
  pinMode(latch_pin, INPUT);
  pinMode(clock_pin, INPUT);

  // Attach interrupt for when the NES is latching the current state.
  // The reason why we can't use the built in SPI interrupt is because
  // the NES latches data on the RISING edge, while the SPI latch is
  // on the FALLING edge.
  attachInterrupt(digitalPinToInterrupt(latch_pin), latchState, RISING);
  
  // The LatchSate-pin should be on the SPI SS pin.
  // The SPIMODE must also be set to trigger the intterupt on
  // the RISING edge of SS


  // HYOPTHESIS 1:
  // Normally, the SS pin is kept HIGH when slave is NOT selected.
  // During this time we are able to write to the SPDR register.
  // So if the NES latch is connected to SS, it will be kept low,
  // making the SPDR unwritable.
  // So as long as we can attach an interrupt on the latch pin on the
  // rising edge, we can set the SPDR register when it is HIGH and before it
  // goes LOW again.
  // Data in SPDR can only be shifted out while SS is low
  // which should work very well in this case.
}

void loop() {
  
  // Listen to transmitted data and write it to the SPDR register
  // as long as it's not currently being read by the NES
  //if (digitalRead(latch_pin) == HIGH) {
  //  SPDR = 0xff;
  //}

  // Store current input state
  // EDIT: Input byte should be stored in the SPDR register.
  // That way we can use the SPI protocol and send the data
  // to the NES acting as a slave.
}

void latchState() {

  // If this interrupt is being run, it means that the NES would
  // normally latch data in the NES controller shift register now.
  // In this case we're telling the SPI that we're unselecting it.
  // While the SPI is unselected, we're able to edit the SPDR register
  // which contains the data that will be presented on the
  // SPI MISO line.

  // Simply put the NES controller input in the SPDR register.
  SPDR = data;
}

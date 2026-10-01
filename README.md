# Rube Goldberg Car Pusher

The motor remains off after reset until an RFID tag is presented to the MFRC522 reader. Any readable tag starts the motor through the relay. When the car presses the target button, the Arduino debounces the switch, turns off the relay, and sends an NEC infrared signal (address `0x00`, command `0x01`) with two repeats. The motor stays off until another tag is presented.

## Arduino pins

- D4: relay module `IN` (active-high)
- D2: target pushbutton to GND, using `INPUT_PULLUP`
- D3: IR LED through a 220-ohm resistor to GND
- D10: MFRC522 SDA/SS
- D9: MFRC522 RST
- D11 (MOSI), D12 (MISO), D13 (SCK): MFRC522 SPI bus
- 3.3 V and GND: MFRC522 power (use 3.3 V, not 5 V)

## Physical motor wiring

Use the kit's low-voltage 3–6 V DC motor and fan blade. Power the relay module from the Uno's 5 V and GND, and connect `IN` to D4. Wire the motor through the relay's `COM` and `NO` contacts, using a separate regulated 3–6 V supply for the motor. Put the included 1N4007 diode across the motor terminals, with its banded end at motor positive. Do not connect the motor to an Arduino I/O pin or power it from the Uno's 5 V output. Secure the motor and propeller before powering it, and keep fingers clear while it is running.

The Wokwi diagram shows a simulated DC motor switched by the relay and a red LED as a visual stand-in for the IR emitter. It models the motor's on/off state, but not the propeller pushing a car or visible infrared light. The Uno pin and target-button behavior match the sketch.

The sketch uses the Arduino `IRremote` and `MFRC522` libraries. Arduino CLI users can install them with `arduino-cli lib install IRremote` and `arduino-cli lib install MFRC522`; Wokwi uses the included `libraries.txt` dependency list.

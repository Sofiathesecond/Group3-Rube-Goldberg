// Rube Goldberg car pusher
// The propeller motor runs after startup until the car presses the target button.
// The relay module switches motor power; the motor does not connect to an Uno pin.
#define IR_SEND_PIN 3
#define DISABLE_CODE_FOR_RECEIVER

const byte RELAY_PIN = 4;
const byte TARGET_BUTTON_PIN = 2;
const byte RFID_SS_PIN = 10;
const byte RFID_RST_PIN = 9;
const unsigned long DEBOUNCE_MS = 30;

#include <IRremote.hpp>
#include <SPI.h>
#include <MFRC522.h>

MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);

bool motorRunning = false;
int lastRawButtonState = HIGH;
int stableButtonState = HIGH;
unsigned long lastButtonChangeMs = 0;

void setMotor(bool on) {
  motorRunning = on;
  // The Wokwi relay module used here is active-high.
  digitalWrite(RELAY_PIN, on ? HIGH : LOW);
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(TARGET_BUTTON_PIN, INPUT_PULLUP);
  SPI.begin();
  rfid.PCD_Init();
  Serial.begin(9600);

  setMotor(false);
  Serial.println("Present an RFID tag to start the motor.");
}

void loop() {
  if (!motorRunning && rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    setMotor(true);
    Serial.print("RFID tag detected; motor started. UID:");
    for (byte i = 0; i < rfid.uid.size; i++) {
      Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(rfid.uid.uidByte[i], HEX);
    }
    Serial.println();
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }

  const unsigned long now = millis();
  const int rawButtonState = digitalRead(TARGET_BUTTON_PIN);

  if (rawButtonState != lastRawButtonState) {
    lastRawButtonState = rawButtonState;
    lastButtonChangeMs = now;
  }

  if (now - lastButtonChangeMs >= DEBOUNCE_MS &&
      rawButtonState != stableButtonState) {
    stableButtonState = rawButtonState;

    // INPUT_PULLUP makes a pressed target button read LOW.
    if (stableButtonState == LOW && motorRunning) {
      setMotor(false);
      Serial.println("Target button pressed: motor stopped.");
      Serial.println("Sending NEC IR signal: address 0x00, command 0x01.");
      IrSender.sendNEC(0x00, 0x01, 2);
    }
  }
}

#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
  15, 0,                  // 13 Buttons, 0 Hat Switches
  false, false, false,    // No X, Y, or Z Axis
  false, false, false,    // No Rx, Ry, or Rz
  false, false,           // No Rudder or Throttle
  false, false, false     // No Accelerator, Brake, or Steering
);

// --- Digital Pins ---
const int signalLeftPin = 2;
const int signalRightPin = 3;
const int lightsLowBeamPin = 4;
const int lightsParkPin = 5;
const int lightsTogglePin = 6;
const int highBeamOutPin = 7;
const int highBeamInPin = 8;
const int washerPin = 14;
const int wiperHighPin = 15;

// --- Analog Pins ---
const int wiperSignalPin = A0;

void setup() {
  Serial.begin(9600);

  Joystick.begin();

  // --- Signal Stalk ---
  pinMode(signalLeftPin, INPUT_PULLUP);
  pinMode(signalRightPin, INPUT_PULLUP);
  pinMode(lightsLowBeamPin, INPUT_PULLUP);
  pinMode(lightsParkPin, INPUT_PULLUP);
  pinMode(lightsTogglePin, INPUT_PULLUP);
  pinMode(highBeamInPin, INPUT_PULLUP);
  pinMode(highBeamOutPin, INPUT_PULLUP);

  // --- Wiper Stalk ---
  pinMode(washerPin, INPUT_PULLUP);
  pinMode(wiperHighPin, INPUT_PULLUP);
}

void loop() {
  // --- Signal Stalk ---
  Joystick.setButton(0, !digitalRead(signalLeftPin));
  Joystick.setButton(1, !digitalRead(signalRightPin));

  // --- lightsLowBeam Low/High Logic ---
  if (!digitalRead(lightsLowBeamPin)) {
    Joystick.setButton(2, 1); // LowBeam : ON
    Joystick.setButton(3, 0); // Park    : OFF
  } 
  else if (!digitalRead(lightsParkPin)) {
    Joystick.setButton(2, 0); // LowBeam : OFF
    Joystick.setButton(3, 1); // Park    : ON
  } 
  else {
    Joystick.setButton(2, 0); // LowBeam : OFF
    Joystick.setButton(3, 0); // Park    : OFF
  }

  Joystick.setButton(4, !digitalRead(lightsTogglePin));
  Joystick.setButton(5, !digitalRead(highBeamInPin));
  Joystick.setButton(6, !digitalRead(highBeamOutPin));
  Joystick.setButton(7, !digitalRead(washerPin));

  // --- Intermittent Wiper Ladder --- Neutral ADC: 1023
  int wiperADC = analogRead(wiperSignalPin);
  Joystick.setButton(8,  (wiperADC > 250 && wiperADC < 300)); // INT 1 ADC: 287
  Joystick.setButton(9,  (wiperADC > 200 && wiperADC < 250)); // INT 2 ADC: 225
  Joystick.setButton(10, (wiperADC > 150 && wiperADC < 200)); // INT 3 ADC: 169
  Joystick.setButton(11, (wiperADC > 100 && wiperADC < 150)); // INT 4 ADC: 118
  Joystick.setButton(12, (wiperADC >  50 && wiperADC < 100)); // INT 5 ADC: 73
  // Serial.print("DEBUG | wiperValue: ");Serial.println(wiperValue);

  // --- Wiper Low/High Logic ---
  if (!digitalRead(wiperHighPin)) {
    Joystick.setButton(13, 0); // Low  : OFF
    Joystick.setButton(14, 1); // High : ON
  } 
  else if (wiperADC > 10 && wiperADC < 50) { // WiperLow ADC: 31
    Joystick.setButton(13, 1); // Low  : ON
    Joystick.setButton(14, 0); // High : OFF
  } 
  else {
    Joystick.setButton(13, 0); // Low  : OFF
    Joystick.setButton(14, 0); // High : OFF
  }

  delay(10);
}
#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
  13, 1,                  // 13 Buttons, 1 Hat Switches
  false, false, false,    // No X, Y, or Z Axis
  false, false, false,    // No Rx, Ry, or Rz
  false, false,           // No Rudder or Throttle
  false, false, false     // No Accelerator, Brake, or Steering
);

// --- Digital Pins ---
const int volumeUpPin = 2;
const int volumeDownPin = 3;
const int favoriteUpPin = 4;
const int favoriteDownPin = 5;
const int dpadUpPin = 6;
const int dpadDownPin = 7;
const int dpadLeftPin = 8;
const int dpadRightPin = 9;
const int dpadSelectPin = 10;
const int voicePin = 14;
const int mutePin = 16;

// --- Analog Pins ---
const int paddleSignalPin = A0;
const int cruiseSignalPin = A1;

void setup() {
  Serial.begin(9600);

  Joystick.begin();

  // --- Media Buttons ---
  pinMode(favoriteUpPin, INPUT_PULLUP);
  pinMode(favoriteDownPin, INPUT_PULLUP);
  pinMode(volumeUpPin, INPUT_PULLUP);
  pinMode(volumeDownPin, INPUT_PULLUP);

  // --- Right D-Pad Module ---
  pinMode(dpadUpPin, INPUT_PULLUP);
  pinMode(dpadDownPin, INPUT_PULLUP);
  pinMode(dpadLeftPin, INPUT_PULLUP);
  pinMode(dpadRightPin, INPUT_PULLUP);
  pinMode(dpadSelectPin, INPUT_PULLUP);
  pinMode(voicePin, INPUT_PULLUP);
  pinMode(mutePin, INPUT_PULLUP);
}

void loop() {
  // --- Media Buttons ---
  Joystick.setButton(0, !digitalRead(favoriteUpPin));
  Joystick.setButton(1, !digitalRead(favoriteDownPin));
  Joystick.setButton(2, !digitalRead(volumeUpPin));
  Joystick.setButton(3, !digitalRead(volumeDownPin));

  // --- 8-way D-Pad (4 Buttons) + Select ---
  bool dpadUpState    = !digitalRead(dpadUpPin);
  bool dpadDownState  = !digitalRead(dpadDownPin); 
  bool dpadLeftState  = !digitalRead(dpadLeftPin);
  bool dpadRightState = !digitalRead(dpadRightPin);
  int dpadHatAngle = -1;

  if (dpadUpState && dpadRightState)        dpadHatAngle = 45;
  else if (dpadDownState && dpadRightState) dpadHatAngle = 135;
  else if (dpadDownState && dpadLeftState)  dpadHatAngle = 225;
  else if (dpadUpState && dpadLeftState)    dpadHatAngle = 315;
  else if (dpadUpState)                     dpadHatAngle = 0;
  else if (dpadRightState)                  dpadHatAngle = 90;
  else if (dpadDownState)                   dpadHatAngle = 180;
  else if (dpadLeftState)                   dpadHatAngle = 270;
  else                                      dpadHatAngle = -1;
  Joystick.setHatSwitch(0, dpadHatAngle);
  Joystick.setButton(10, !digitalRead(dpadSelectPin));

  // --- Phone Buttons ---
  Joystick.setButton(11, !digitalRead(voicePin));
  Joystick.setButton(12, !digitalRead(mutePin));

  // --- Paddle Shifters --- Neutral ADC: 390
  int paddleADC = analogRead(paddleSignalPin);
  Joystick.setButton(4, (paddleADC > 115 && paddleADC < 280));  // Shift Down ADC: 170
  Joystick.setButton(5, (paddleADC >  30 && paddleADC < 115));  // Shift Up ADC: 60
  // Serial.print("DEBUG | paddleValue: ");Serial.println(paddleValue);

  // --- Cruise Control Buttons --- Neutral ADC: 480
  int cruiseADC = analogRead(cruiseSignalPin);
  Joystick.setButton(6, (cruiseADC > 145 && cruiseADC < 220));   // Toggle ADC: 165
  Joystick.setButton(7, (cruiseADC > 320 && cruiseADC < 420));   // Cancel ADC: 126
  Joystick.setButton(8, (cruiseADC > 100 && cruiseADC < 145));   // RES+ ADC: 360
  Joystick.setButton(9, (cruiseADC > 220 && cruiseADC < 320));   // SET- ADC: 275
  // Serial.print("DEBUG | cruiseValue: ");Serial.println(cruiseValue);

  delay(10);
}
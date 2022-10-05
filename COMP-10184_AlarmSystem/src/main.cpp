// COMP-10184 – Mohawk College
// IoT Project #1 - Alarm System
//
// ------------------------------------------------
// I have completed stage 5 of the requirements.
// ------------------------------------------------
// Demonstrates a real-world application for an IoT device which acts as a motion sensor alarm.
// When motion is detected, the alarm goes into a countdown mode for 10 seconds and if the
// button is pressed within that time the alarm is disabled, otherwise the alarm will activate.
// After disabling, the button can be pressed once again to re-enable the alarm after a short delay.
//
// @author  Robert Zaranek
// @id      001161598
//
// I, Robert Zaranek, 001161598 certify that this material is my original work.
// No other person's work has been used without due acknowledgement.

#include <Arduino.h>

// digital input pin definitions
#define PIN_PIR D5
#define PIN_BUTTON D6

// Flipped boolean for use with IO devices for better readability
#define ACTIVATED false

// The amount of quarter seconds in 10 seconds (4*10)
#define TEN_SECONDS_FOUR_BLINKS_PER_SECOND 40

// Grace period before starting PIR sensor again
#define BUTTON_DELAY 3000

// The amount of ms the light is on during a blink interval
#define LIGHT_ON_DELAY 50

// The amount of ms the light is off during a blink interval
#define LIGHT_OFF_DELAY 200

// define all possible alarm states.
#define ALARM_DISABLED 0
#define ALARM_ENABLE 1
#define ALARM_COUNTDOWN 2
#define ALARM_ACTIVE 3


// An int value representing the current state of the alarm application
int iAlarmState = ALARM_ENABLE;

// Flag for activating the button only once after it is pressed
bool bIsButtonPressed = false;

// Flag to activate console logging
bool bDebugMode = false;

// *************************************************************
/// @brief Toggles the alarm state between enabled and disabled after the button is pressed.
/// @param bButton The state of the button input
void toggleAlarmStateWithButton(bool bButton) {
  if (bButton == ACTIVATED && !bIsButtonPressed) {
    bIsButtonPressed = true;
    if (iAlarmState == ALARM_ENABLE) {
      iAlarmState = ALARM_DISABLED;
      if (bDebugMode) Serial.println("Alarm Disabled(2)!");
    } else if (iAlarmState == ALARM_DISABLED) {
      iAlarmState = ALARM_ENABLE;
      if (bDebugMode) Serial.println("Alarm Enabled!");
      delay(BUTTON_DELAY);
    }
  } else if (bButton != ACTIVATED) {
    bIsButtonPressed = false;
  }
}

/// @brief Begins the alarm countdown while allowing for the button to be pressed to disable the alarm.
void alarmCountdown() {
  bIsButtonPressed = false;
  iAlarmState = ALARM_COUNTDOWN;
  if (bDebugMode) Serial.println("Countdown Started!");

  for (int i = 0; i < TEN_SECONDS_FOUR_BLINKS_PER_SECOND; i++) {
    bool bButton = digitalRead(PIN_BUTTON);
    if (bDebugMode) Serial.println(bButton);
    if (bButton == ACTIVATED) {
      bIsButtonPressed = true;
      break;
    }
    
    digitalWrite(LED_BUILTIN, ACTIVATED);
    delay(LIGHT_ON_DELAY);
    digitalWrite(LED_BUILTIN, !ACTIVATED);
    delay(LIGHT_OFF_DELAY);
  }

  if (bIsButtonPressed) {
    iAlarmState = ALARM_DISABLED;
    if (bDebugMode) Serial.println("Alarm Disabled(1)!");
  } else {
    iAlarmState = ALARM_ACTIVE;
    if (bDebugMode) Serial.println("Alarm Active!");
  }
}

// *************************************************************
void setup() {
  // configure the USB serial monitor
  Serial.begin(115200);
  // configure the LED output
  pinMode(LED_BUILTIN, OUTPUT);
  // PIR sensor is an INPUT
  pinMode(PIN_PIR, INPUT);
  // Button is an INPUT
  pinMode(PIN_BUTTON, INPUT_PULLUP);
}

// *************************************************************
void loop() {
  // The current state of the PIR sensor
  bool bPIR;
  // The current state of the button
  bool bButton;
  bPIR = digitalRead(PIN_PIR);
  bButton = digitalRead(PIN_BUTTON);

  switch (iAlarmState)
  {
  case ALARM_ACTIVE:
      digitalWrite(LED_BUILTIN, ACTIVATED);
    break;

  default:
      digitalWrite(LED_BUILTIN, !ACTIVATED);
      toggleAlarmStateWithButton(bButton);
      
      if (bPIR && iAlarmState == ALARM_ENABLE) {
        alarmCountdown();
      }
    break;
  }
}
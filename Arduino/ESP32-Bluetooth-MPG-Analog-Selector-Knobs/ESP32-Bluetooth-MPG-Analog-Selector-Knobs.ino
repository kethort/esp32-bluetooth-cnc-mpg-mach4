#include <ESP32Encoder.h>
#include <BleCombo.h>

#define ANA0 34
#define ANA1 35
#define MPGEN 25
#define ENCA 2
#define ENCB 4

ESP32Encoder encoder;

int lastCounts = 0;
uint8_t currentAxis = KEY_F1;
uint8_t currentInc = KEY_F7;
bool axisChanged;
bool incChanged;
bool wasDisabled;

int getJogWheelDirection() {
  int encDirection = 0;
  if (lastCounts != encoder.getCount()) {
    if (lastCounts < encoder.getCount()) 
      encDirection = 1;
    else 
      encDirection = -1;
    lastCounts = encoder.getCount();
  }
  return encDirection;
}

void setup() {
  Serial.begin(115200);

  pinMode(MPGEN, INPUT_PULLUP);
  
  encoder.attachFullQuad(ENCA, ENCB);
  encoder.clearCount();

  Mouse.begin();
  Keyboard.begin();
}

void getSelectedAxis() {
  uint8_t selectedAxisKey;
  int adc0Val = analogRead(34);

  if (adc0Val >= 0 && adc0Val < 700) {
    selectedAxisKey = KEY_F1; 
  } else if (adc0Val >= 700 && adc0Val < 1400) {
    selectedAxisKey = KEY_F2;
  } else if (adc0Val >= 1400 && adc0Val < 2100) {
    selectedAxisKey = KEY_F3;
  } else if (adc0Val >= 2100 && adc0Val < 2800) {
    selectedAxisKey = KEY_F4;
  } else if (adc0Val >= 2800 && adc0Val < 3500) {
    selectedAxisKey = KEY_F5;
  } else if (adc0Val >= 3500 && adc0Val < 4096) {
    selectedAxisKey = KEY_F6;
  }
  
  if (selectedAxisKey && currentAxis != selectedAxisKey) {
    axisChanged = true;
    currentAxis = selectedAxisKey;
  }
}

void getSelectedInc() {
  uint8_t selectedIncKey;
  int adc1Val = analogRead(35);

  if (adc1Val >= 0 && adc1Val < 1400) {
    selectedIncKey = KEY_F7;
  } else if (adc1Val >= 1400 && adc1Val < 2800) {
    selectedIncKey = KEY_F8;
  } else if (adc1Val >= 2800 && adc1Val < 4095) {
    selectedIncKey = KEY_F9;
  } 

  if (selectedIncKey && currentInc != selectedIncKey) {
    incChanged = true;
    currentInc = selectedIncKey;
  }
}

void loop() {
  // change axis and inc when mpg is enabled
  if(!digitalRead(MPGEN)) {
    if (wasDisabled) {
      wasDisabled = false;   
      if(Keyboard.isConnected()) {
        Keyboard.press(KEY_F10);
        delay(200); 
        Keyboard.release(KEY_F10); 
      }
      axisChanged = incChanged = true;
    }
  } else if (digitalRead(MPGEN)) {
    if (!wasDisabled) {
      wasDisabled = true;
      if(Keyboard.isConnected()) {
        Keyboard.press(KEY_F11);
        delay(200);
        Keyboard.release(KEY_F11); 
      }
    }
  }
  
  int encoderDirection = getJogWheelDirection();

  getSelectedAxis();
  getSelectedInc();
 
  if (Keyboard.isConnected()) {
    if (encoderDirection != 0) {
      Mouse.move(0, 0, encoderDirection);
    }
    
    if (axisChanged) {
      axisChanged = false;
      Keyboard.press(currentAxis);
      delay(200);
      Keyboard.release(currentAxis);
    }
    
    if (incChanged) {
      incChanged = false;
      Keyboard.press(currentInc);
      delay(200);
      Keyboard.release(currentInc);
    }
  }
}

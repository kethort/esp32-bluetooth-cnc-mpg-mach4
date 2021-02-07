#include <ESP32Encoder.h>
#include <BleCombo.h>

#define XAXIS 13
#define YAXIS 14
#define ZAXIS 27
#define AAXIS 32
#define BAXIS 35
#define CAXIS 34
#define INC1 15
#define INC2 21
#define INC3 22
#define MPGEN 25
#define ENCA 2
#define ENCB 4

ESP32Encoder encoder;

int lastCounts = 0;
byte currentAxis;
byte currentInc;
bool axisChanged;
bool incChanged;
bool wasDisabled;
int pins[] = {XAXIS, YAXIS, ZAXIS, AAXIS, BAXIS, CAXIS, INC1, INC2, INC3, MPGEN};

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
  //Serial.begin(115200);
  
  for(int i = 0; i < sizeof(pins)/sizeof(pins[0]); i++) {
    pinMode(pins[i], INPUT_PULLUP);
  } 

  encoder.attachFullQuad(ENCA, ENCB);
  encoder.clearCount();

  Mouse.begin();
  Keyboard.begin();
}

void loop() { 
  for(int i = 0; i < sizeof(pins)/sizeof(pins[0]) - 1; i++)
  {
    if (!digitalRead(pins[i]) && i < 6 && currentAxis != pins[i]) {
      currentAxis = pins[i];
      axisChanged = true;
    }

    if (!digitalRead(pins[i]) && i > 5 && currentInc != pins[i]) {
      currentInc = pins[i];
      incChanged = true;
    }
  }
  
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

  if(Keyboard.isConnected()) {
    if (encoderDirection != 0) {
      Mouse.move(0, 0, encoderDirection);
    }
    if (axisChanged) {
      axisChanged = false;
      if (!digitalRead(XAXIS)) {
        Keyboard.press(KEY_F1);
        delay(200);
        Keyboard.release(KEY_F1);
      }
      if (!digitalRead(YAXIS)) {
        Keyboard.press(KEY_F2);
        delay(200);
        Keyboard.release(KEY_F2);
      }
      if (!digitalRead(ZAXIS)) {
        Keyboard.press(KEY_F3);
        delay(200);
        Keyboard.release(KEY_F3);
      }
      if (!digitalRead(AAXIS)) {
        Keyboard.press(KEY_F4);
        delay(200);
        Keyboard.release(KEY_F4);
      }
      if (!digitalRead(BAXIS)) {
        Keyboard.press(KEY_F5);
        delay(200);
        Keyboard.release(KEY_F5);
      }
      if (!digitalRead(CAXIS)) {
        Keyboard.press(KEY_F6);
        delay(200);
        Keyboard.release(KEY_F6);
      }
    }
    if (incChanged) {
      incChanged = false;
      if (!digitalRead(INC1)) {
        Keyboard.press(KEY_F7);
        delay(200);
        Keyboard.release(KEY_F7);
      }
      if (!digitalRead(INC2)) {
        Keyboard.press(KEY_F8);
        delay(200);
        Keyboard.release(KEY_F8);
      }
      if (!digitalRead(INC3)) {
        Keyboard.press(KEY_F9);
        delay(200);
        Keyboard.release(KEY_F9);
      }
    }
  }
}

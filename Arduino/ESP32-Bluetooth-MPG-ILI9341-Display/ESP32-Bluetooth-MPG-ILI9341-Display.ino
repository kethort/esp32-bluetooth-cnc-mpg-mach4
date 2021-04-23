#include <ESP32Encoder.h>
#include <BleCombo.h>

#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h> 

TFT_eSPI tft = TFT_eSPI();       

// This is the file name used to store the touch coordinate
// calibration data. Change the name to start a new calibration.
#define CALIBRATION_FILE "/TouchCalData4"

// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false

#define AXISBUTTON_W 72
#define AXISBUTTON_H 50

#define XAXISBUTTON_X 5
#define XAXISBUTTON_Y 35

#define YAXISBUTTON_X 85
#define YAXISBUTTON_Y 35

#define ZAXISBUTTON_X 164
#define ZAXISBUTTON_Y 35

#define AAXISBUTTON_X 5
#define AAXISBUTTON_Y 95

#define BAXISBUTTON_X 85
#define BAXISBUTTON_Y 95

#define CAXISBUTTON_X 164
#define CAXISBUTTON_Y 95

#define INC1BUTTON_X 5
#define INC1BUTTON_Y 195

#define INC2BUTTON_X 85
#define INC2BUTTON_Y 195

#define INC3BUTTON_X 164
#define INC3BUTTON_Y 195

#define PREVBUTTON_X 5
#define PREVBUTTON_Y 265

#define NEXTBUTTON_X 164
#define NEXTBUTTON_Y 265

#define MAINBUTTON_X 85
#define MAINBUTTON_Y 265

#define MPGEN 32
#define SCRLED 27

//#define DEBUG // enables Serial

int lastCounts = 0;
byte lastAxis;
byte lastInc;
bool wasDisabled;

ESP32Encoder encoder;

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
#ifdef DEBUG
  Serial.begin(115200);
#endif

  tft.init();
  tft.setRotation(0);
  touch_calibrate();
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);

  pinMode(MPGEN, INPUT_PULLUP);
  pinMode(SCRLED, OUTPUT);
  digitalWrite(SCRLED, HIGH);

  encoder.attachFullQuad(4, 2);
  encoder.clearCount();

  Mouse.begin();
  Keyboard.begin();

  drawIncButtons();
  drawAxisButtons();
}

void loop() {  
  getTouchMPG();

  if(!digitalRead(MPGEN)) {
    if (wasDisabled) {
      wasDisabled = false;   
      if(Keyboard.isConnected()) {
        Keyboard.press(KEY_F10);
        delay(200); 
        Keyboard.release(KEY_F10); 
      }
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
  }
}

void getTouchMPG() {
  uint16_t x, y;

  if (tft.getTouch(&x, &y))
  { 
    if ((x > XAXISBUTTON_X) && (x < (XAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > XAXISBUTTON_Y) && (y <= (XAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(XAXISBUTTON_X, XAXISBUTTON_Y, "X");
        if(Keyboard.isConnected()) {
          Keyboard.press(KEY_F1);
          delay(200);
          Keyboard.release(KEY_F1);
        }
        lastAxis = 0;
      }
    }

    if ((x > YAXISBUTTON_X) && (x < (YAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > YAXISBUTTON_Y) && (y <= (YAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(YAXISBUTTON_X, YAXISBUTTON_Y, "Y");
        if(Keyboard.isConnected()) {
          Keyboard.press(KEY_F2);
          delay(200);
          Keyboard.release(KEY_F2);
        }
        lastAxis = 1;
      }
    }

    if ((x > ZAXISBUTTON_X) && (x < (ZAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > ZAXISBUTTON_Y) && (y <= (ZAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(ZAXISBUTTON_X, ZAXISBUTTON_Y, "Z");
        if(Keyboard.isConnected()) {
          Keyboard.press(KEY_F3);
          delay(200);
          Keyboard.release(KEY_F3);
        }
        lastAxis = 2;
      }
    }

    if ((x > AAXISBUTTON_X) && (x < (AAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > AAXISBUTTON_Y) && (y <= (AAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(AAXISBUTTON_X, AAXISBUTTON_Y, "A");
        if(Keyboard.isConnected()) {
          Keyboard.press(KEY_F4);
          delay(200);
          Keyboard.release(KEY_F4);
        }
        lastAxis = 3;
      }
    }

    if ((x > BAXISBUTTON_X) && (x < (BAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > BAXISBUTTON_Y) && (y <= (BAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(BAXISBUTTON_X, BAXISBUTTON_Y, "B");
        if(Keyboard.isConnected()) {
          Keyboard.press(KEY_F5);
          delay(200);
          Keyboard.release(KEY_F5);
        }
        lastAxis = 4;
      }
    }

    if ((x > CAXISBUTTON_X) && (x < (CAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > CAXISBUTTON_Y) && (y <= (CAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(CAXISBUTTON_X, CAXISBUTTON_Y, "C");
        if(Keyboard.isConnected()) {
          Keyboard.press(KEY_F6);
          delay(200);
          Keyboard.release(KEY_F6);
        }
        lastAxis = 5;
      }
    }

    if ((x > INC1BUTTON_X) && (x < (INC1BUTTON_X + AXISBUTTON_W))) {
      if ((y > INC1BUTTON_Y) && (y <= (INC1BUTTON_Y + AXISBUTTON_H))) {
        selectInc(INC1BUTTON_X, INC1BUTTON_Y, "0.001");
        if(Keyboard.isConnected()) {
          Keyboard.press(KEY_F7);
          delay(200);
          Keyboard.release(KEY_F7);
        }
        lastInc = 7;
      }
    }

    if ((x > INC2BUTTON_X) && (x < (INC2BUTTON_X + AXISBUTTON_W))) {
      if ((y > INC2BUTTON_Y) && (y <= (INC2BUTTON_Y + AXISBUTTON_H))) {
        selectInc(INC2BUTTON_X, INC2BUTTON_Y, "0.01");
        if(Keyboard.isConnected()) {
          Keyboard.press(KEY_F8);
          delay(200);
          Keyboard.release(KEY_F8);
        }
        lastInc = 8;
      }
    }

    if ((x > INC3BUTTON_X) && (x < (INC3BUTTON_X + AXISBUTTON_W))) {
      if ((y > INC3BUTTON_Y) && (y <= (INC3BUTTON_Y + AXISBUTTON_H))) {
        selectInc(INC3BUTTON_X, INC3BUTTON_Y, "0.1");
        if(Keyboard.isConnected()) {
          Keyboard.press(KEY_F9);
          delay(200);
          Keyboard.release(KEY_F9);
        }
        lastInc = 9;
      }
    }
  }
}

void drawIncButtons() {
  tft.setTextColor(TFT_WHITE);
  tft.drawString("MPG Increment:", 125, 170);

  tft.setTextColor(TFT_BLACK);
  tft.fillRect(INC1BUTTON_X, INC1BUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.drawString("0.001", INC1BUTTON_X + (AXISBUTTON_W / 2), INC1BUTTON_Y + (AXISBUTTON_H / 2));
  
  tft.fillRect(INC2BUTTON_X, INC2BUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.drawString("0.01", INC2BUTTON_X + (AXISBUTTON_W / 2), INC2BUTTON_Y + (AXISBUTTON_H / 2));
  
  tft.fillRect(INC3BUTTON_X, INC3BUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.drawString("0.1", INC3BUTTON_X + (AXISBUTTON_W / 2), INC3BUTTON_Y + (AXISBUTTON_H / 2));
}

void drawAxisButtons() {
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("MPG Axis:", 120, 20);

  tft.setTextColor(TFT_BLACK);
  tft.fillRect(XAXISBUTTON_X, XAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.drawString("X", XAXISBUTTON_X + (AXISBUTTON_W / 2), XAXISBUTTON_Y + (AXISBUTTON_H / 2)); 
  
  tft.fillRect(YAXISBUTTON_X, YAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.drawString("Y", YAXISBUTTON_X + (AXISBUTTON_W / 2), YAXISBUTTON_Y + (AXISBUTTON_H / 2)); 
  
  tft.fillRect(ZAXISBUTTON_X, ZAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.drawString("Z", ZAXISBUTTON_X + (AXISBUTTON_W / 2), ZAXISBUTTON_Y + (AXISBUTTON_H / 2)); 
  
  tft.fillRect(AAXISBUTTON_X, AAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.drawString("A", AAXISBUTTON_X + (AXISBUTTON_W / 2), AAXISBUTTON_Y + (AXISBUTTON_H / 2)); 
  
  tft.fillRect(BAXISBUTTON_X, BAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.drawString("B", BAXISBUTTON_X + (AXISBUTTON_W / 2), BAXISBUTTON_Y + (AXISBUTTON_H / 2)); 
  
  tft.fillRect(CAXISBUTTON_X, CAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.drawString("C", CAXISBUTTON_X + (AXISBUTTON_W / 2), CAXISBUTTON_Y + (AXISBUTTON_H / 2));
}

void selectAxis(int32_t x, int32_t y, const char *axisID) {
  drawAxisButtons();
  tft.fillRect(x, y, AXISBUTTON_W, AXISBUTTON_H, TFT_GREEN);
  tft.setTextColor(TFT_BLACK);
  tft.drawString(axisID, x + (AXISBUTTON_W / 2), y + (AXISBUTTON_H / 2));
}

void selectInc(int32_t x, int32_t y, const char *axisID) {
  drawIncButtons();
  tft.fillRect(x, y, AXISBUTTON_W, AXISBUTTON_H, TFT_GREEN);
  tft.setTextColor(TFT_BLACK);
  tft.drawString(axisID, x + (AXISBUTTON_W / 2), y + (AXISBUTTON_H / 2));
}

void touch_calibrate() {
  
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check file system exists
  if (!SPIFFS.begin()) {
#ifdef DEBUG
    Serial.println("Formating file system");
#endif
    SPIFFS.format();
    SPIFFS.begin();
  }

  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL)
    {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CALIBRATION_FILE);
    }
    else
    {
      File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
    tft.setTouch(calData);
  } else {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}

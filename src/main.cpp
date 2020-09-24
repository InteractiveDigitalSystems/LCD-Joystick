#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

int JOYX_PIN = 39;
int JOYY_PIN = 36;
int JOYBTN_PIN = 34;

int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x3F, lcdColumns, lcdRows);

void setup() {
  Serial.begin(9600);

  // initialize LCD
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();
  lcd.setCursor(0, 0);
}

bool isInDeadZone(std::pair<int,int> joystick) {
  int joyXCenterVal = 1715;
  int joyYCenterVal = 1776;
  int offset = 150;
  bool deadX = joystick.first < joyXCenterVal + offset && joystick.first > joyXCenterVal - offset;
  bool deadY = joystick.second < joyYCenterVal + offset && joystick.second > joyYCenterVal - offset;
  return deadX && deadY;
}

void formatPrintJoystickValues(std::pair<int,int> joystick) {
  String s = "X: " + String(joystick.first) + ", Y: " + String(joystick.second);
  Serial.println(s);
}

std::pair<int, int> getJoystickValues() {
  int xVal = analogRead(JOYX_PIN);
  int yVal = analogRead(JOYY_PIN);
  return std::make_pair(xVal, yVal);
}

void loop() {
  std::pair<int, int> joystick = getJoystickValues();
  bool isMoving = isInDeadZone(joystick) == false;
  if (isMoving) {
    formatPrintJoystickValues(joystick);
    String s = "X: " + String(joystick.first) + ", Y: " + String(joystick.second);
    lcd.print(s);
  } else {
    lcd.print("Not moving");
  }
  lcd.clear();
}

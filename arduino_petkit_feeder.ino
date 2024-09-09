#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>

#include "motor_group.h"
#include "scale.h"
#include "scheduler.h"

/*
 * Program for Petkit dog feeder
 */


/**
 * PIN numbers
 * 
 * PWM pins must be 3, 5, 6, 9, 10, 11
 **/

const int PIN_TEST_BUTTON = 2;
const uint8_t I2C_LCD_ADDRESS = 0x3F;

/**
 * Other constants
 **/
const unsigned long TEST_BUTTON_DEBOUNCE_MILLISEC = 500UL;
const unsigned long LCD_REFRESH_MILLISEC = 1000UL;
const unsigned long MAIN_LOOP_MILLISEC = 200UL;
const unsigned long ALARM_POLL_MILLISEC = 1000UL;
const unsigned long LCD_FLASH_MILLISEC = 1000UL;

const int LOW_FOOD_PERCENTAGE = 10;

/**
 * Devices
 **/
Scheduler rtc;
Scale scale;
LiquidCrystal_PCF8574 lcd(I2C_LCD_ADDRESS);
MotorGroup motorGroup;

/**
 * Global variables
 **/

unsigned long testButtonLastClickTs = 0UL;
unsigned long lcdLastRefreshTs = 0UL;
unsigned long alarmPollLastTs = 0UL;
unsigned long lcdFlashLastTs = 0UL;
bool lcdOn = true;

void setupSerial() {
  Serial.begin(9600);
  Serial.println("LCD...");

  // wait on Serial to be available on Leonardo
  while (!Serial)
    ;
}

void setupLCD() {
  byte dotOff[] = { 0b00000, 0b01110, 0b10001, 0b10001,
                    0b10001, 0b01110, 0b00000, 0b00000 };
  byte dotOn[] = { 0b00000, 0b01110, 0b11111, 0b11111,
                  0b11111, 0b01110, 0b00000, 0b00000 };
  int error;

  Serial.print("Probing for PCF8574");
  Serial.println(I2C_LCD_ADDRESS);

  //LCD
  Wire.begin();
  Wire.beginTransmission(I2C_LCD_ADDRESS);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");
    lcd.begin(16, 2);  // initialize the lcd
    lcd.setBacklight(255);
    lcd.home();

    lcd.createChar(1, dotOff);
    lcd.createChar(2, dotOn);

  } else {
    Serial.println(": LCD not found.");
  }
  
}

void setupPins() {
  pinMode(PIN_TEST_BUTTON, INPUT_PULLUP);
}

void setup()                    // run once, when the sketch starts
{
  setupPins();
  motorGroup.init();
  setupSerial();
  setupLCD();
  scale.init();
  rtc.init();
}

void loop2() {
  // updateLCD();
  String dateTime = rtc.printNow();
  lcd.setCursor(0, 0);
  lcd.print(dateTime);

  if (Serial.available()) {
    char input = Serial.read();
    if (input == 'u') rtc.setTime();  // update RTC time
  }
}

void printDateTime() {
  String now = rtc.printNow();
  String alarm = rtc.printAlarm();
  lcd.setCursor(0, 0);
  lcd.print(""+now+" next:"+alarm);
}

void loop()
{
  unsigned long mainLoopStartTs = millis();

  // Test button
  if (digitalRead(PIN_TEST_BUTTON) == LOW) {
    if(millis() - testButtonLastClickTs > TEST_BUTTON_DEBOUNCE_MILLISEC) {
      motorGroup.toggleDoor();
      testButtonLastClickTs = millis();
    }
  }

  // motor group
  motorGroup.run();

  // scale
  int scaleValue = scale.getWeightPercentage();
  if(scaleValue < LOW_FOOD_PERCENTAGE) {
    if(millis() - lcdFlashLastTs > LCD_FLASH_MILLISEC) {
      if(lcdOn) {
        lcd.setBacklight(0);
        lcdOn = false;
      }else{
        lcd.setBacklight(255);
        lcdOn = true;
      }
    }
  }else{
    lcd.setBacklight(255);
    lcdOn = true;
  }

  // scheduler
  if(millis() - alarmPollLastTs > ALARM_POLL_MILLISEC) {
    if(rtc.checkAlarm()) {
      motorGroup.toggleDoor();
      rtc.clearAlarm();
    }
    alarmPollLastTs = millis();
  }

  // LCD
  if (millis() - lcdLastRefreshTs > LCD_REFRESH_MILLISEC) {
    lcd.clear();

    printDateTime();

    lcd.setCursor(0, 1);
    lcd.print(String(scaleValue)+"%");

    lcdLastRefreshTs = millis();
  }

  unsigned long now = millis();
  if(now < mainLoopStartTs + MAIN_LOOP_MILLISEC) {
    delay(mainLoopStartTs + MAIN_LOOP_MILLISEC - now);
  }
}
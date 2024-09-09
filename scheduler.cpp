#include <Arduino.h>
#include "scheduler.h"

void Scheduler::init() {
    rtc.begin();
    rtc.disable32K();
    rtc.clearAlarm(2);
}

void Scheduler::setTime() {
    // ask user to enter new date and time
    const char txt[6][15] = { "year [4-digit]", "month [1~12]", "day [1~31]",
                            "hours [0~23]", "minutes [0~59]", "seconds [0~59]"};
    String str = "";
    long newDate[6];

    while (Serial.available()) {
    Serial.read();  // clear serial buffer
    }

    for (int i = 0; i < 6; i++) {

    Serial.print("Enter ");
    Serial.print(txt[i]);
    Serial.print(": ");

    while (!Serial.available()) {
        ; // wait for user input
    }

    str = Serial.readString();  // read user input
    newDate[i] = str.toInt();   // convert user input to number and save to array

    Serial.println(newDate[i]); // show user input
    }

    // update RTC
    rtc.adjust(DateTime(newDate[0], newDate[1], newDate[2], newDate[3], newDate[4], newDate[5]));
    Serial.println("RTC Updated!");
}

String Scheduler::printDateTime(DateTime datetime)
{
    char value[30];

    int ss = datetime.second();
    int mm = datetime.minute();
    int hh = datetime.hour();
    int DD = datetime.dayOfTheWeek();
    int dd = datetime.day();
    int MM = datetime.month();
    int yyyy = datetime.year();

    sprintf(value, "%02d-%02d-%04d %02d:%02d:%02d",
            dd, MM, yyyy, hh, mm, ss);
    
    return String(value);
}

String Scheduler::printTime(DateTime datetime)
{
    char value[6];

    int mm = datetime.minute();
    int hh = datetime.hour();

    sprintf(value, "%02d:%02d",
            hh, mm);
    
    return String(value);
}

void Scheduler::setAlarm() {
    DateTime now = rtc.now();
    DateTime nowNoDate (0, 0, 0, now.hour(), now.minute(), 0);
    int smallestTimeIndex = -1;

    if(sizeof(schedules) == 0) {
        return;
    }

    int rows = sizeof(schedules) / sizeof(schedules[0]);

    // find the smallest time after now
    for(int i = 0; i < rows; i++) {
        DateTime s (0, 0, 0, schedules[i][0], schedules[i][1], 0);
        if(s > nowNoDate && (smallestTimeIndex == -1 || s < DateTime(0, 0, 0, schedules[smallestTimeIndex][0], schedules[smallestTimeIndex][1], 0))) {
            smallestTimeIndex = i;
        }
    }

    if(smallestTimeIndex == -1) {
        // use the smallest time
        for(int i = 0; i < rows; i++) {
            DateTime s (0, 0, 0, schedules[i][0], schedules[i][1], 0);
            if(smallestTimeIndex == -1 || s < DateTime(0, 0, 0, schedules[smallestTimeIndex][0], schedules[smallestTimeIndex][1], 0)) {
                smallestTimeIndex = i;
            }
        }
    }
    
    rtc.setAlarm2(DateTime(0, 0, 0, schedules[smallestTimeIndex][0], schedules[smallestTimeIndex][1], 0), DS3231_A2_Hour);
    isAlarmSet = true;
}

bool Scheduler::checkAlarm() {
    if(!isAlarmSet) {
        setAlarm();
    }

    return rtc.alarmFired(2);
}

String Scheduler::printNow() {
    return printTime(rtc.now());
}

void Scheduler::clearAlarm() {
    rtc.clearAlarm(2);
    isAlarmSet = false;
}

String Scheduler::printAlarm() {
    if(isAlarmSet) {
        return printTime(rtc.getAlarm2());
    }else {
        return "N/A";
    }
}
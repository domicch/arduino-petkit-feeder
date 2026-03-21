#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <RTClib.h>

class Scheduler {
    private:
        RTC_DS3231 rtc;

        int schedules[20][2] = { // [hour, minute] - do NOT use {0, 0} (midnight is reserved as empty/uninitialized marker)
            {8, 0},
        };

        bool isAlarmSet = false;
        void setAlarm();
        String printDateTime(DateTime datetime);
        String printTime(DateTime datetime);

    public:
        void init();
        void setTime();
        String printNow();
        String printAlarm();
        bool checkAlarm();
        void clearAlarm();
};

#endif
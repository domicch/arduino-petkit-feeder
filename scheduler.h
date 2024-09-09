#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <RTClib.h>

class Scheduler {
    private:
        RTC_DS3231 rtc;

        int schedules[2][2] = { // [hour, minute]
            {7, 0},
            {19, 0},
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
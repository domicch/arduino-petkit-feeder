#ifndef MOTOR_GROUP_H
#define MOTOR_GROUP_H

#include "queue.h"

class MotorGroup {
  private:
    
    /**
     * PIN numbers
     * 
     * PWM pins must be 3, 5, 6, 9, 10, 11
     **/
    // Food motor
    const int PIN_FOOD_MOTOR_DIR_1 = 3;
    const int PIN_FOOD_MOTOR_DIR_2 = 4;
    const int PIN_FOOD_MOTOR_SPEED = 9;
    const int PIN_FOOD_MOTOR_SENSOR = 8;

    // Door motor
    const int PIN_DOOR_MOTOR_DIR_1 = 5;
    const int PIN_DOOR_MOTOR_DIR_2 = 6;
    const int PIN_DOOR_MOTOR_SPEED = 10;
    const int PIN_DOOR_MOTOR_SENSOR = 7;

    const int FOOD_MOTOR_SPEED = 100;
    const int DOOR_MOTOR_SPEED = 190;

    // States
    static const int STATE_INIT = 0;
    static const int STATE_IDLE = 1;

    static const int STATE_TO_OPEN_DOOR = 10;
    static const int STATE_TO_CLOSE_DOOR = 12;

    static const int STATE_FEEDER_MOVING = 21;
    
    int currentState = STATE_INIT;

    Queue commandQueue = Queue();

    const unsigned long DOOR_DELAY_MILLISEC = 1000;
    unsigned long doorDelayStartTs = 0UL;

    int feederLastSensorState;
    int feederRotationCounter;
    int feederTargetRotations;

    void moveDoorMotorOpen();

    void moveDoorMotorClose();

    void stopDoorMotor();

    bool isDoorSensorBlocked();

    void changeState(int toState);

    int getFeederSensorState();

    void moveFeederMotor();

    void stopFeederMotor();

  public:
    
    void init();

    void run();

    void toggleDoor();

    int getFoodSensorValue();

    int getDoorSensorValue();
};

#endif
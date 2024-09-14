#include <Arduino.h>
#include "motor_group.h"


void MotorGroup::moveDoorMotorOpen() {
    analogWrite(PIN_DOOR_MOTOR_SPEED, DOOR_MOTOR_SPEED);
    digitalWrite(PIN_DOOR_MOTOR_DIR_1, HIGH);
    digitalWrite(PIN_DOOR_MOTOR_DIR_2, LOW);
}

void MotorGroup::moveDoorMotorClose() {
    analogWrite(PIN_DOOR_MOTOR_SPEED, DOOR_MOTOR_SPEED);
    digitalWrite(PIN_DOOR_MOTOR_DIR_1, LOW);
    digitalWrite(PIN_DOOR_MOTOR_DIR_2, HIGH);
}

void MotorGroup::stopDoorMotor() {
    digitalWrite(PIN_DOOR_MOTOR_DIR_1, LOW);
    digitalWrite(PIN_DOOR_MOTOR_DIR_2, LOW);
}

bool MotorGroup::isDoorSensorBlocked() {
    return digitalRead(PIN_DOOR_MOTOR_SENSOR) == 0;
}

int MotorGroup::getFeederSensorState () {
    return digitalRead(PIN_FOOD_MOTOR_SENSOR);
}

void MotorGroup::moveFeederMotor() {
    analogWrite(PIN_FOOD_MOTOR_SPEED, FOOD_MOTOR_SPEED);
    digitalWrite(PIN_FOOD_MOTOR_DIR_1, HIGH);
    digitalWrite(PIN_FOOD_MOTOR_DIR_2, LOW);
}

void MotorGroup::stopFeederMotor() {
    digitalWrite(PIN_FOOD_MOTOR_DIR_1, LOW);
    digitalWrite(PIN_FOOD_MOTOR_DIR_2, LOW);
}

void MotorGroup::changeState(int toState) {
    switch(toState) {
    case STATE_TO_OPEN_DOOR:
        doorDelayStartTs = millis();
        moveDoorMotorOpen();
        break;
    case STATE_TO_CLOSE_DOOR:
        doorDelayStartTs = millis();
        moveDoorMotorClose();
        break;
    case STATE_FEEDER_MOVING:
        feederLastSensorState = getFeederSensorState();
        feederRotationCounter = 0;
        moveFeederMotor();
        break;
    }
    
    currentState = toState;
}


// Public
void MotorGroup::init() {
    pinMode(PIN_FOOD_MOTOR_DIR_1, OUTPUT);
    pinMode(PIN_FOOD_MOTOR_DIR_2, OUTPUT);
    pinMode(PIN_FOOD_MOTOR_SPEED, OUTPUT);
    pinMode(PIN_DOOR_MOTOR_DIR_1, OUTPUT);
    pinMode(PIN_DOOR_MOTOR_DIR_2, OUTPUT);
    pinMode(PIN_DOOR_MOTOR_SPEED, OUTPUT);
    pinMode(PIN_FOOD_MOTOR_SENSOR, INPUT);
    pinMode(PIN_DOOR_MOTOR_SENSOR, INPUT);
}

void MotorGroup::run() {
    switch(currentState) {
    case STATE_INIT:
        changeState(STATE_IDLE);
        break;
    case STATE_IDLE:
        if(!commandQueue.isEmpty()) {
            changeState(commandQueue.dequeue());
        }
        break;
    case STATE_TO_OPEN_DOOR:
        if(millis() - doorDelayStartTs > DOOR_DELAY_MILLISEC) {
            if(!isDoorSensorBlocked()) {
                stopDoorMotor();
                changeState(STATE_IDLE);
            }
        }
        break;
    case STATE_TO_CLOSE_DOOR:
        if(millis() - doorDelayStartTs > DOOR_DELAY_MILLISEC) {
            if(!isDoorSensorBlocked()) {
                stopDoorMotor();
                changeState(STATE_IDLE);
            }
        }
        break;
    case STATE_FEEDER_MOVING:
        int feederSensorState = getFeederSensorState();
        if(feederSensorState != feederLastSensorState) {
            feederRotationCounter++;
            feederLastSensorState = feederSensorState;
        }
        if(feederRotationCounter >= FEEDER_TARGET_ROTATIONS) {
            stopFeederMotor();
            changeState(STATE_IDLE);
        }
        break;

    }
}

void MotorGroup::toggleDoor() {
    commandQueue.enqueue(STATE_TO_OPEN_DOOR);
    commandQueue.enqueue(STATE_FEEDER_MOVING);
    commandQueue.enqueue(STATE_TO_CLOSE_DOOR);
    
}

int MotorGroup::getFoodSensorValue() {
    return digitalRead(PIN_FOOD_MOTOR_SENSOR);
}

int MotorGroup::getDoorSensorValue() {
    return digitalRead(PIN_DOOR_MOTOR_SENSOR);
}

/*******************************************************************************
 * BluetoothRobot.ino
 * 2016-05-26
 * Mike Quigley
 * 
 * Control code for a small robot, with basic bluetooth driving controls
 ******************************************************************************/
#include <RobotMotor.h>

RobotMotor *leftMotor = new RobotMotor(7, 8, 6);
RobotMotor *rightMotor = new RobotMotor(9, 10, 11);

void processLeftTick()
{
    leftMotor->processTick();
}

void processRightTick()
{
    rightMotor->processTick();
}

void setup()
{
    pinMode(2, INPUT);
    pinMode(3, INPUT);
    attachInterrupt(digitalPinToInterrupt(2), processLeftTick, FALLING);
    attachInterrupt(digitalPinToInterrupt(3), processRightTick, FALLING);

    //For now, just tell the left motor to move forward 20 ticks as a test
    leftMotor->move(-20, 255);
}

void loop()
{
    leftMotor->refresh();
    rightMotor->refresh();
}

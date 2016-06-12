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
char btIn[10];

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

    //The Bluetooth module is connected through the UART, and works like a serial port
    Serial.begin(9600);
    //Because the motors won't be running while the program waits for input, we can
    //use blocking I/O functions. Set timeout to 1 hour.
    Serial.setTimeout(3600000);

    //Motor movements commented out for bluetooth testing
    //rightMotor->move(-40, 255);
}

void loop()
{
    Serial.readBytesUntil(0x0A, btIn, 10);
    Serial.print(btIn);
    
    leftMotor->refresh();
    rightMotor->refresh();
}

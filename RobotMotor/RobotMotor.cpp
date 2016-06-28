/*******************************************************************************
 * RobotMotor.cpp
 * 2016.05.23
 * Mike Quigley
 * 
 * Class to encapsulate a motor/rotary encoder pair for robotics use
 ******************************************************************************/
 #include <RobotMotor.h>
 #include <arduino.h>

//Constructor
RobotMotor::RobotMotor(unsigned char red, unsigned char black, unsigned char enable)
{
    this->red = red;
    this->black = black;
    this->enable = enable;
    this->running = false;
    
    pinMode(red, OUTPUT);
    pinMode(black, OUTPUT);
    pinMode(enable, OUTPUT);
}

//Get current state
bool RobotMotor::isStopped()
{
    return !running;
}

//Check tick counter and stop motor if required.
void RobotMotor::refresh()
{
    if(ticks == 0)
    {
        digitalWrite(enable, 0);
        running = false;
    }
}

//Start the motor. It will run for a distance given in encoder ticks.
//Use a negative distance to run in reverse. Speed is controlled through
//PWM on the enable pin.
void RobotMotor::move(int dist, int speed)
{
    if(dist > 0)
    {
        digitalWrite(red, 0);
        digitalWrite(black, 1);
    }
    else
    {
        digitalWrite(red, 1);
        digitalWrite(black, 0);
    }
    ticks = abs(dist);
    running = true;
    analogWrite(enable, speed);
}

//When using this class, create an interrupt handler that wraps this method
void RobotMotor::processTick()
{
    ticks--;
}

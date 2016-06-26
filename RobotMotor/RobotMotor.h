/*******************************************************************************
 * RobotMotor.cpp
 * 2016.05.23
 * Mike Quigley
 * 
 * Class to encapsulate a motor/rotary encoder pair for robotics use
 ******************************************************************************/
 
class RobotMotor
{
    unsigned char red, black, enable;       //IO Pins
    unsigned char ticks;                    //Counter
    bool running;                           //True if motor is turned on, false if it's off
    public:
    
        //Constructor
        RobotMotor(unsigned char red, unsigned char black, unsigned char enable);
    
        //Get current state
        bool isStopped();
        
        //Check tick counter and stop motor if required.
        void refresh();
        
        //Start the motor. It will run for a distance given in encoder ticks.
        //Use a negative distance to run in reverse. Speed is controlled through
        //PWM on the enable pin.
        void move(int dist, int speed);
        
        //When using this class, create an interrupt handler that wraps this method
        void processTick();
};

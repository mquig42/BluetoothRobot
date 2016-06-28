/*******************************************************************************
 * BluetoothRobot.ino
 * 2016-05-26
 * Mike Quigley
 * 
 * Control code for a small robot, with basic bluetooth driving controls
 ******************************************************************************/
#include <RobotMotor.h>
#include <string.h>

#define COMMAND_BUFFER 16           //Number of commands to store

enum state {WAIT, RUN};
state s = WAIT;

RobotMotor *leftMotor = new RobotMotor(7, 8, 6);
RobotMotor *rightMotor = new RobotMotor(9, 10, 11);

char commands[COMMAND_BUFFER][11];  //Array of C strings to store program.
uint8_t pc = 0;                     //program counter

void processLeftTick()
{
    leftMotor->processTick();
}

void processRightTick()
{
    rightMotor->processTick();
}

/** getNextToken
 *  Used for parsing delimited strings
 *  Will find the substring beginning at start, and ending before next occurrence of sep
 *  Substring will be copied to out
 *  returns index after sep, or 0 if the end of str was reached
 */
uint8_t getNextToken(char *str, char sep, uint8_t start, char *out)
{
    int i = 0;
    while((str[i+start])&&(str[i+start] != sep))
    {
        out[i] = str[i+start];
        i++;
    }
    //Add null terminator
    out[i] = '\0';
    if(str[i+start])
        return i + start + 1;
    return 0;
}

/** clearProgram
 *  Erases all stored commands and resets pc to 0
 */
void clearProgram()
{
    for(pc=0;pc<COMMAND_BUFFER;pc++)
    {
        commands[pc][0] = '\0';
    }
    pc = 0;
}

void execMotorCommand()
{
    RobotMotor *curMotor = leftMotor;
    if(commands[pc][0] == 'R')
    {
        curMotor = rightMotor;
    }
    if(curMotor->isStopped())
    {
        Serial.println(commands[pc]);
        uint8_t pos = 2;
        char token[8];
        pos = getNextToken(commands[pc], ',', pos, token);
        int dist = atoi(token);
        getNextToken(commands[pc], ',', pos, token);
        int spd = atoi(token);
        curMotor->move(dist, spd);
        pc++;
    }
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
}

void loop()
{
    switch(s)
    {
        case WAIT:
            Serial.readBytesUntil(';', commands[pc], 10);
            if(strcmp(commands[pc], "RUN") == 0)
            {
                pc = 0;
                s = RUN;
                break;
            }
            if(strcmp(commands[pc], "CLEAR") == 0)
            {
                clearProgram();
                break;
            }
            pc++;
            break;
        case RUN:
            if(strcmp(commands[pc], "RUN") == 0)
            {
                //Wait for motors to stop before we change states. Motors will not refresh in wait state
                if((leftMotor->isStopped())&&(rightMotor->isStopped()))
                {
                    s = WAIT;
                    Serial.println("READY:");
                }
                break;
            }
            if(strcmp(commands[pc], "PAUSE") == 0)
            {
                //Wait for motors to stop before moving to next instruction
                if((leftMotor->isStopped())&&(rightMotor->isStopped()))
                {
                    pc++;
                }
                break;
            }
            //If we reach this point, the current instruction is a motor movement
            execMotorCommand();
            break;
    }
    
    leftMotor->refresh();
    rightMotor->refresh();
}

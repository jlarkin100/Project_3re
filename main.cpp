//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"

#include "engine.h"
#include "display.h"
#include "wiper.h"
#include "userInterface.h"

#define SYSTEM_TIME_INCREMENT_MS        10
//=====[Main function, the program entry point after power on or reset]========

int main()
{   

    engineInit();
    userInterfaceDisplayInit();
    while (true) {
        engineUpdate();
        if(getEngineState()){
            wiperUpdate();
            userInterfaceDisplayUpdate();
        }
        delay(SYSTEM_TIME_INCREMENT_MS);
    }
}



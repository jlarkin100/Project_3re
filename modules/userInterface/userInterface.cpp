#include "mbed.h"
#include "arm_book_lib.h"

#include "display.h"
#include "wiper.h"
#include "userInterface.h"

#define DISPLAY_REFRESH_TIME_MS         1000
#define SYSTEM_TIME_INCREMENT_MS        10


void userInterfaceDisplayInit()
{
    displayInit();
     
    displayCharPositionWrite ( 0,0 );
    displayStringWrite( "Mode:" );

    displayCharPositionWrite ( 0,1 );
    displayStringWrite( "INTDelay:" );
}

void userInterfaceDisplayUpdate()
{
    static int accumulatedDisplayTime = 0;
    
    if( accumulatedDisplayTime >= DISPLAY_REFRESH_TIME_MS ) {

        accumulatedDisplayTime = 0;
        displayCharPositionWrite ( 11, 1);
        switch (getDelay()) {
        case 0:
            displayStringWrite( "SHORT" );
            break;
        case 1:
            displayStringWrite( "MEDIUM" );
            break;
        case 2:
            displayStringWrite( "LONG" );
            break;
        default:
            displayStringWrite( "" );
        }

        displayCharPositionWrite ( 7, 0);
        switch (getMode()) {
        case 0:
            displayStringWrite( "HI" );
            break;
        case 1:
            displayStringWrite( "OFF" );
            break;
        case 2:
            displayStringWrite( "LO" );
            break;
        case 3:
            displayStringWrite( "INT" );
            break;
        default:
            displayStringWrite( "" );
        }

        
    } 
    else {
        accumulatedDisplayTime =
            accumulatedDisplayTime + SYSTEM_TIME_INCREMENT_MS;        
    } 
}
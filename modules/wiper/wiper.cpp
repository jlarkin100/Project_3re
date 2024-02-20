#include "mbed.h"
#include "arm_book_lib.h"

#include "display.h"
#include "wiper.h"
#include <functional>

#define TIME_INCREMENT_MS                       10
#define SHORT                                   3000
#define MEDIUM                                  6000
#define LONG                                    8000
#define DUTY_MIN 0.015
#define DUTY_MAX 0.110
#define PERIOD 0.02

AnalogIn stateKnob(A0);
AnalogIn delaySelector(A1);
PwmOut wiper(PF_9);


int selected_delaytime = 0;
typedef enum {
    HI_MODE,
    LO_MODE,
    INT_MODE,
    OFF_MODE
} wiperState_t;

wiperState_t wiperState;

static void wiperModeHI();
static void wiperModeOFF();
static void wiperModeLO();
static void wiperModeINT();
static void setMode();
static void wiperloop(int MIN, int MAX);
static void cycle();
static void setDelay();


int stateNumber = 0;
int Delay_no = 0;

int getMode(){
    return stateNumber;
}

static void cycle(float min, float max, float period, int delay){
    wiper.period(period); // 20ms period
    
    wiper.write(max);
    delay(delay);
    wiper.write(min);
    delay(delay);
    
}


static void wiperSweep(float DUTY, int delay){
    wiper.period(PERIOD); // 20ms period
    wiper.write(DUTY);
    delay(delay);
}

void wiperUpdate() {
    setDelay();
    setMode();
    switch (wiperState) {
    case HI_MODE:
        stateNumber = 0;
        cycle(0.015, 0.115, 0.02, 280);
    
        break;
    
    case LO_MODE:
        
        stateNumber = 1;
        cycle(0.015, 0.115, 0.02, 370);
        break;

    case INT_MODE:

        stateNumber = 2;
        cycle(0.015, 0.115, 0.02, 370);
        delay(selected_delaytime);
        break;
    case OFF_MODE:

        stateNumber = 3;
        break;
    default:

        stateNumber = 3;
        break;

    }
    
}

static void setMode(){//Issue with potentiometer voltage
    float WiperModeValue = stateKnob.read();
    if(WiperModeValue <= 1.00 && WiperModeValue > 0.75) {
        wiperState = HI_MODE;
    }
    else if(WiperModeValue > 0.50 && WiperModeValue <= 0.75) {
        wiperState = LO_MODE;
    }
    else if(WiperModeValue > 0.25 && WiperModeValue <= 0.50) {
        wiperState = INT_MODE;
    }
    else if(WiperModeValue <= 0.25) {
        wiperState = OFF_MODE;
    }
    else  {
        wiperState = OFF_MODE;
    }
}
int getDelay(){
    return Delay_no;    
}
static void setDelay(){
    float knob_reading = delaySelector.read();

    if(knob_reading <= 0.33){
        selected_delaytime = SHORT;
        Delay_no = 0;
    }
    else if(knob_reading > 0.33 && knob_reading <= 0.66){
        selected_delaytime = MEDIUM;
        Delay_no = 1;
    }
    else{
        selected_delaytime = LONG;
        Delay_no = 2;
    }
}
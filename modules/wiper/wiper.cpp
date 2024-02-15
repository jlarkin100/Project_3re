#include "mbed.h"
#include "arm_book_lib.h"

#include "display.h"
#include "wiper.h"

#define TIME_INCREMENT_MS                       10
#define DEBOUNCE_BUTTON_TIME_MS                 40
#define SHORT                                   10
#define MEDIUM                                  50
#define LONG                                    100
#define DUTY_MIN 0.015
#define DUTY_MAX 0.110
#define PERIOD 0.02

AnalogIn stateKnob(A0);
AnalogIn delaySelector(A1);
PwmOut wiper(PF_9);


int selected_delaytime = 0;
typedef enum {
    HI_MODE,
    OFF_MODE,
    LO_MODE,
    INT_MODE
} wiperState_t;

wiperState_t wiperState;

static void wiperModeHI();
static void wiperModeOFF();
static void wiperModeLO();
static void wiperModeINT();
static void setMode();
static void wiperloop(int MIN, int MAX);
static void test();



//gets delay time based on intermittent delay time selector potentiometer 


int getMode(){
    return wiperState;
}

static void test(){
    wiper.period(PERIOD); // 20ms period
    wiper.write(DUTY_MIN);
    
    wiper.write(0.015);
    delay(2000);
    wiper.write(0.110);
    delay(2000);
    
}


static void wiperSweep(float DUTY, int delay){
    wiper.period(PERIOD); // 20ms period
    wiper.write(DUTY);
    delay(delay);
}

void wiperUpdate() {
    getDelay();
    setMode();
    switch (wiperState) {
    case HI_MODE:
        test();
        wiperState = HI_MODE;
        break;
    
    case LO_MODE:
        test();
        wiperState = LO_MODE;
        break;

    case INT_MODE:
        test();
        wiperState = INT_MODE;
        break;
    case OFF_MODE:
        wiperState = OFF_MODE;
        break;
    default:
        wiperState = OFF_MODE;
        break;
    }
    
}

static void setMode(){//Issue with potentiometer voltage
    float WiperModeValue = stateKnob.read();
    if(WiperModeValue <= 1.00 && WiperModeValue > 0.75) {
        wiperState = HI_MODE;
    }
    if(WiperModeValue > 0.50 && WiperModeValue <= 0.75) {
        wiperState = LO_MODE;
    }
    if(WiperModeValue > 0.25 && WiperModeValue <= 0.50) {
        wiperState = INT_MODE;
    }
    if(WiperModeValue <= 0.25) {
        wiperState = OFF_MODE;
    }
    else {
        wiperState = HI_MODE;
    }
}
int getDelay(){
    int Delay_no = 0;   //The number corresponding to the delay type: SHORT, MEDIUM, LONG
    float knob_reading = delaySelector.read();

    if(knob_reading <= 3.33){
        selected_delaytime = SHORT;
        Delay_no = 0;
    }
    if(knob_reading > 3.33 && knob_reading <= 6.66){
        selected_delaytime = MEDIUM;
        Delay_no = 1;
    }
    else{
        selected_delaytime = LONG;
        Delay_no = 2;
    }
    return Delay_no;
}

static void wiperModeHI()
{
    wiperSweep(DUTY_MIN, 2000);
    wiperSweep(DUTY_MAX, 2000);
}

static void wiperModeLO()
{
    wiperSweep(DUTY_MIN, 2000);
    wiperSweep(DUTY_MAX, 2000);
}

static void wiperModeINT()
{

    wiperSweep(DUTY_MIN, 2000);
    wiperSweep(DUTY_MAX, 2000);
}

static void wiperModeOFF()
{
    wiper.write(0);
}

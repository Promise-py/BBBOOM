#ifndef __BATTERY_H
#define __BATTERY_H

#include <stdint.h>
#include "main.h"
#include "adc.h"

#define Resistance 4.7
typedef struct BATTERY{

    uint8_t Cells_Num;
    float Cells_Voltage[6];
    float Battery_Voltage;

    float Alarm_Voltage; //3.8v
    float CutOff_Voltage; //3.7v
}BATTERY;

extern BATTERY Battery[3];
#endif

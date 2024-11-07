#ifndef __BATTERY_H
#define __BATTERY_H

#include <stdint.h>
#include "main.h"
#include "adc.h"

#define Resistance 1.5
typedef struct BATTERY{

    uint8_t Cells_Num;
    float Cells_Voltage[6];
    float Battery_Voltage;

    float Alarm_Voltage; //3.8v
    float CutOff_Voltage; //3.7v
}BATTERY;
void Battery_Init(BATTERY *battery,float alarm_voltage,float cut_off_voltage);
extern BATTERY Battery[3];
extern unsigned int ADC1_value[8];//IN 8-14
extern unsigned int ADC2_value[8];//IN 0-7
#endif

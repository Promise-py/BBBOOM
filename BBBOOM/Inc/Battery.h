#ifndef __BATTERY_H
#define __BATTERY_H

#include <stdint.h>


#define Resistance 1.53//分压电阻 分压系数

//电池状态定义
#define READY       1 //就绪
#define EXHAUST     2 //耗尽（断电）
#define ALARM       3 //报警
#define NUL         4 //无电池
#define ERR         5 //错误

typedef struct BATTERY{

    uint8_t Cells_Num;
    uint8_t Battery_State;
    float Cells_Voltage[6];
    float Battery_Voltage;

    float Alarm_Voltage; //3.8v
    float CutOff_Voltage; //3.7v

    uint8_t Alarm_Flag; //报警标志
    uint8_t Alarm_Cells; //报警电池号
    uint8_t Exhaust_Flag; //耗尽标志
}BATTERY;

void Battery_Init(BATTERY *battery,float alarm_voltage,float cut_off_voltage);
void Battery_Refresh(BATTERY *battery);
void Battery_ON(uint8_t CHx,uint8_t mode);
extern BATTERY Battery[3];
extern unsigned int ADC1_value[8];//IN 8-14
extern unsigned int ADC2_value[8];//IN 0-7
#endif

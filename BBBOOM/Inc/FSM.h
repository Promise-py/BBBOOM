#ifndef __FSM_H
#define __FSM_H

#include <stdint.h>
#include "main.h"
#include "Battery.h"
#include "oled.h"


typedef struct BB_FSM
{
    uint8_t BB_State;       //bboom当前状态
    uint8_t BB_CHx;         //当前使用的通道
    uint8_t Show_CellNum;   //当前显示的电芯序号
    uint8_t ERR_CHx;        //错误电池的通道
}BB_FSM;

void FSM_StateRefresh(BATTERY *battery,BB_FSM *fsm);
extern BB_FSM BB_fsm;
#endif


#ifndef __UI_H
#define __UI_H

#include "oled.h"
#include "Battery.h"
#include "oledfont.h" 
#include "FSM.h"

typedef struct UI_STATE
{
    uint8_t mode;
    uint8_t CHx;
    uint8_t Cells_No;
}UI_STATE;

void UI_ShowRUAGH(void);
void UI_Normal(BATTERY *battery,BB_FSM *bb_fsm);
#endif



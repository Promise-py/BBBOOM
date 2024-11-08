#ifndef __UI_H__
#define __UI_H__

#include "oled.h"
#include "Battery.h"
#include "oledfont.h"

typedef struct UI_STATE
{
    uint8_t mode;
    uint8_t CHx;
    uint8_t Cells_No;
}UI_STATE;

extern char ch1[];
void UI_ShowRUAGH(void);
void UI_Normal(BATTERY *battery, uint8_t CHx, uint8_t xS, float cell_v, float alaV,float cutV);
#endif

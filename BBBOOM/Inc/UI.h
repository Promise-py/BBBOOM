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
void OLED_ShowRUAGH(void);
void Show_Normal_UI(BATTERY *battery, uint8_t CHx, uint8_t xS, float alaV,float cutV);
#endif

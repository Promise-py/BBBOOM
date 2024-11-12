#include "FSM.h"
#include "UI.h"
#include "tim.h"

BB_FSM BB_fsm;


//刷新数据
void BB_FSM_Refresh(BB_FSM *fsm)
{
    fsm->BB_CHx=1;
    fsm->BB_State=0;
    fsm->Show_CellNum=1;
    fsm->ERR_CHx=0;
}

void FSM_StateRefresh(BATTERY *battery,BB_FSM *fsm)
{
    uint8_t i;
    BB_FSM_Refresh(fsm);
    Battery_Refresh(battery);
    for ( i = 0; i < 3; i++)
    {
        if(battery[i].Battery_State==ALARM)fsm->BB_State=ALARM;
        else if(battery[i].Battery_State==EXHAUST)fsm->BB_State=EXHAUST;
        else if(battery[i].Battery_State==ERR)
        {
            fsm->BB_State=ERR;
            fsm->ERR_CHx=i+1;
            break;
        }
    }
    
    if(!fsm->BB_State)fsm->BB_State=READY;

    UI_Normal(battery,fsm);
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) 
{
    if (htim==&htim1)
    {
        i2c_state++;
        if(BB_fsm.Show_CellNum==1)
        {
            FSM_StateRefresh(Battery,&BB_fsm);
            BB_fsm.Show_CellNum++;

        }

        else if(BB_fsm.Show_CellNum==6)
        {
            UI_Normal(Battery,&BB_fsm);
            BB_fsm.Show_CellNum=1;
        }

        else
        {
            UI_Normal(Battery,&BB_fsm);
            BB_fsm.Show_CellNum++;
        }
    }
}

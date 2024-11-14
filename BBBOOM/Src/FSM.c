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
    fsm->Exhaust_CHx=0;
    fsm->CHx_Num=0;
}

void FSM_StateRefresh(BATTERY *battery,BB_FSM *fsm)
{
    uint8_t i;
    BB_FSM_Refresh(fsm);
    Battery_Refresh(battery);
    for ( i = 0; i < 3; i++)
    {
        if(battery[i].Battery_State==ALARM)
        {
            fsm->BB_State=ALARM;
            fsm->ALARM_CHx=i+1;
            Battery_ON(i+1,1);
            fsm->CHx_Num++;
        }

        else if(battery[i].Battery_State==EXHAUST&&battery[i].Exhaust_Flag==0)
        {
            fsm->BB_State=EXHAUST;
            fsm->Exhaust_CHx=i+1;
            Battery_ON(i+1,0);
            fsm->CHx_Num++;
        }

        else if(battery[i].Battery_State==ERR)
        {
            fsm->BB_State=ERR;
            fsm->ERR_CHx=i+1;
            fsm->CHx_Num++;
            Battery_ON(i+1,0);
            // break;
        }

        else if(battery[i].Battery_State==READY)
        {
            fsm->CHx_Num++;
            Battery_ON(i+1,1);
        }

        else Battery_ON(i+1,0);
    }
    
    if(!fsm->BB_State)fsm->BB_State=READY;

    UI_Normal(battery,fsm);
}


/**
 * @breif:蜂鸣器报警
 * @param:电池数组，状态机结构体
 * @return:无
 */
void FSM_Buzzer(BATTERY *battery,BB_FSM *fsm)
{
    uint8_t i;
    for(i=0;i<3;i++)
    {
        if(battery[i].Battery_State==ALARM&&battery[i].Alarm_Flag==0)
        {
            fsm->Show_CHx=i+1;
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,5000);

            UI_ALARM(battery,fsm);
            fsm->ALARM_Time++;
            break;
        }

        if(battery[i].Battery_State==EXHAUST&&battery[i].Exhaust_Flag==0)
        {
            fsm->Show_CHx=i+1;
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,8000);
            UI_ALARM(battery,fsm);
            fsm->ALARM_Time++;
            break;
        }
    }

    if(fsm->ALARM_Time>5)
    {
        fsm->ALARM_Time=0;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,0);

        if(fsm->BB_State==ALARM)battery[i].Alarm_Flag=1;
        if(fsm->BB_State==EXHAUST)battery[i].Exhaust_Flag=1;
        
        fsm->BB_State=READY;
        
        UI_Normal(battery,fsm);
    }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) 
{
    if (htim==&htim1)
    {
        
        // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,5000);
        if(BB_fsm.BB_State==ALARM)
        {
            FSM_Buzzer(Battery,&BB_fsm);
        }

        if(BB_fsm.BB_State==EXHAUST)
        {
            //oled轮询显示
            FSM_Buzzer(Battery,&BB_fsm);
        }
        
        if(BB_fsm.ALARM_Time==0)UI_Rotate(Battery,&BB_fsm);
        
    }
}


//按键切换UI
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == Button_Pin)
    {
        if(BB_fsm.BB_State==READY)
        {
            __HAL_TIM_SET_COUNTER(&htim1,0);
            BB_fsm.Show_CHx++;
            while(Battery[BB_fsm.Show_CHx-1].Battery_State==NUL || BB_fsm.Show_CHx>3)
            {
                BB_fsm.Show_CHx++;
                if(BB_fsm.Show_CHx>3)
                {
                    BB_fsm.Show_CHx=1;
                    while(Battery[BB_fsm.Show_CHx-1].Battery_State==NUL)BB_fsm.Show_CHx++;
                }
            }
            UI_Normal(Battery,&BB_fsm);
        }

        else if(BB_fsm.BB_State==ALARM||BB_fsm.BB_State==EXHAUST)
        {
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,0);
            Battery[BB_fsm.Exhaust_CHx-1].Exhaust_Flag==1;
            BB_fsm.ALARM_Time=10;
        }
        
    }
}
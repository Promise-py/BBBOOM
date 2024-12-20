#include "UI.h"


UI_STATE UI_State;//当前UI状态

//显示启动院徽
void UI_ShowRUAGH(void)
{
  OLED_ColorTurn(0);
  OLED_DisplayTurn(0);
  OLED_ShowPicture(32,0,70,64,Milk,0);
  OLED_Refresh();
}

void UI_CHx(BATTERY *battery,BB_FSM *bb_fsm)
{
    switch(bb_fsm->Show_CHx)
    {
        case 1:
            if(battery[0].Battery_Voltage>1)
            {
                OLED_ShowString(2,16,"CH1",8,0);
                if(battery[0].Battery_State==ALARM)OLED_DrawLine(0,24,22,24,1);
                else if(battery[0].Battery_State==ERR||battery[0].Battery_State==EXHAUST)OLED_DrawLine(0,20,22,20,0);
            }
            if(battery[1].Battery_Voltage>1)
            {
                OLED_ShowString(2,32,"CH2",8,1);
                if(battery[1].Battery_State==ALARM)OLED_DrawLine(0,40,22,40,1);
                else if(battery[1].Battery_State==ERR||battery[1].Battery_State==EXHAUST)OLED_DrawLine(0,36,22,36,1);
            }
            if(battery[2].Battery_Voltage>1)
            {
                OLED_ShowString(2,48,"CH3",8,1);
                if(battery[2].Battery_State==ALARM)OLED_DrawLine(0,56,22,56,1);
                else if(battery[2].Battery_State==ERR||battery[2].Battery_State==EXHAUST)OLED_DrawLine(0,52,22,52,1);
            }
        break;

        case 2:
            if(battery[0].Battery_Voltage>1)
            {
                OLED_ShowString(2,16,"CH1",8,1);
                if(battery[0].Battery_State==ALARM)OLED_DrawLine(0,24,22,24,1);
                else if(battery[0].Battery_State==ERR||battery[0].Battery_State==EXHAUST)OLED_DrawLine(0,20,22,20,1);
            }
            if(battery[1].Battery_Voltage>1)
            {
                OLED_ShowString(2,32,"CH2",8,0);
                if(battery[1].Battery_State==ALARM)OLED_DrawLine(0,40,22,40,1);
                else if(battery[1].Battery_State==ERR||battery[1].Battery_State==EXHAUST)OLED_DrawLine(0,36,22,36,0);
            }
            if(battery[2].Battery_Voltage>1)
            {
                OLED_ShowString(2,48,"CH3",8,1);
                if(battery[2].Battery_State==ALARM)OLED_DrawLine(0,56,22,56,1);
                else if(battery[2].Battery_State==ERR||battery[2].Battery_State==EXHAUST)OLED_DrawLine(0,52,22,52,1);
            }
        break;

        case 3:
            if(battery[0].Battery_Voltage>1)
            {
                OLED_ShowString(2,16,"CH1",8,1);
                if(battery[0].Battery_State==ALARM)OLED_DrawLine(0,24,22,24,1);
                else if(battery[0].Battery_State==ERR||battery[0].Battery_State==EXHAUST)OLED_DrawLine(0,20,22,20,1);
            }
            if(battery[1].Battery_Voltage>1)
            {
                OLED_ShowString(2,32,"CH2",8,1);
                if(battery[1].Battery_State==ALARM)OLED_DrawLine(0,40,22,40,1);
                else if(battery[1].Battery_State==ERR||battery[1].Battery_State==EXHAUST)OLED_DrawLine(0,36,22,36,1);
            }
            if(battery[2].Battery_Voltage>1)
            {
                OLED_ShowString(2,48,"CH3",8,0);
                if(battery[2].Battery_State==ALARM)OLED_DrawLine(0,56,22,56,1);
                else if(battery[2].Battery_State==ERR||battery[2].Battery_State==EXHAUST)OLED_DrawLine(0,52,22,52,0);
            }
        break;
    }
}

void UI_Normal(BATTERY *battery,BB_FSM *bb_fsm)
{
    OLED_Clear();
    OLED_ColorTurn(0);
    OLED_DisplayTurn(0);

    //显示当前使用通道
    UI_CHx(battery,bb_fsm);
    
    //当前通道
    // OLED_ShowString(0,16,"CH1",8,1);
    // OLED_ShowString(0,32,"CH2",8,1);
    // OLED_ShowString(0,48,"CH3",8,1);

    //显示当前电芯
    OLED_ShowNum(45,0,bb_fsm->Show_CellNum,1,8,1);
    OLED_ShowChar(55,0,'S',8,1);

    float show_voltage = battery[bb_fsm->Show_CHx-1].Cells_Voltage[bb_fsm->Show_CellNum-1];
    //显示当前电芯电压
    OLED_ShowNum(35,13,(int)show_voltage,1,24,1);//个位

    OLED_DrawCircle(48,32,1);//小数点
    
    OLED_ShowNum(50,13,(int)(show_voltage*100)%100,2,24,1);//十&百分位
    
    //显示总电压
    OLED_ShowString(90,0,"Total",8,1);
    OLED_ShowNum(90,18,(int)battery[bb_fsm->Show_CHx-1].Battery_Voltage,2,8,1);//十&个位
    
    OLED_DrawPoint(104,25,1);//小数点

    OLED_ShowNum(108,18,(int)(battery[bb_fsm->Show_CHx-1].Battery_Voltage*100)%100,2,8,1);//十&个位

    OLED_Refresh();
}


void UI_ALARM(BATTERY *battery,BB_FSM *bb_fsm)
{
    OLED_Clear();
    OLED_Refresh();
    switch(bb_fsm->ALARM_CHx)
    {
        case 1:
            if(battery[0].Battery_Voltage>1)OLED_ShowString(0,16,"CH1",8,0);
            if(battery[1].Battery_Voltage>1)OLED_ShowString(0,32,"CH2",8,1);
            if(battery[2].Battery_Voltage>1)OLED_ShowString(0,48,"CH3",8,1);
        break;

        case 2:
            if(battery[0].Battery_Voltage>1)OLED_ShowString(0,16,"CH1",8,1);
            if(battery[1].Battery_Voltage>1)OLED_ShowString(0,32,"CH2",8,0);
            if(battery[2].Battery_Voltage>1)OLED_ShowString(0,48,"CH3",8,1);
        break;

        case 3:
            if(battery[0].Battery_Voltage>1)OLED_ShowString(0,16,"CH1",8,1);
            if(battery[1].Battery_Voltage>1)OLED_ShowString(0,32,"CH2",8,1);
            if(battery[2].Battery_Voltage>1)OLED_ShowString(0,48,"CH3",8,0);
        break;
    }

    //显示当前电芯
    OLED_ShowNum(45,0,battery[bb_fsm->ALARM_CHx-1].Alarm_Cells,1,8,1);
    OLED_ShowChar(55,0,'S',8,1);

    uint8_t cell=battery[bb_fsm->ALARM_CHx-1].Alarm_Cells;
    float show_voltage = battery[bb_fsm->ALARM_CHx-1].Cells_Voltage[cell-1];
    //显示当前电芯电压
    OLED_ShowNum(35,13,(int)show_voltage,1,24,1);//个位

    OLED_DrawCircle(48,32,1);//小数点
    
    OLED_ShowNum(50,13,(int)(show_voltage*100)%100,2,24,1);//十&百分位
    
    //显示总电压
    OLED_ShowString(90,0,"Total",8,1);
    OLED_ShowNum(90,18,(int)battery[bb_fsm->ALARM_CHx-1].Battery_Voltage,2,8,1);//十&个位
    
    OLED_DrawPoint(104,25,1);//小数点

    OLED_ShowNum(108,18,(int)(battery[bb_fsm->ALARM_CHx-1].Battery_Voltage*100)%100,2,8,1);//十&个位

    OLED_Refresh();

}



void UI_Rotate(BATTERY *battery,BB_FSM *bb_fsm)
{
    if(bb_fsm->Show_CHx>3)bb_fsm->Show_CHx=1;
    if(bb_fsm->Show_CHx==0)bb_fsm->Show_CHx=1;

    if(bb_fsm->Show_CellNum==1)
    {
        FSM_StateRefresh(battery,bb_fsm);
        bb_fsm->Show_CellNum++;
    }

    else if(bb_fsm->Show_CellNum>=battery[bb_fsm->Show_CHx-1].Cells_Num)
    {
        UI_Normal(battery,bb_fsm);
        bb_fsm->Show_CellNum=1;
    }

    else
    {
        UI_Normal(battery,bb_fsm);
        bb_fsm->Show_CellNum++;
    }
}




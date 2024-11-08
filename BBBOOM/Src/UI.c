#include "UI.h"

char ch1[]="CH1";

UI_STATE UI_State;//当前UI状态

//显示启动院徽
void UI_ShowRUAGH(void)
{
  OLED_ColorTurn(0);
  OLED_DisplayTurn(0);
  OLED_ShowPicture(32,0,64,64,RUAGH,1);
  OLED_Refresh();
}



void UI_Normal(BATTERY *battery, uint8_t CHx, uint8_t xS, float cell_v, float alaV,float cutV)
{
    
    OLED_ColorTurn(0);
    OLED_DisplayTurn(0);

    //显示当前使用通道
    switch(CHx)
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
    
    //当前通道
    OLED_ShowString(0,16,"CH1",8,1);
    OLED_ShowString(0,32,"CH2",8,1);
    OLED_ShowString(0,48,"CH3",8,1);

    //显示当前电芯
    OLED_ShowNum(45,0,xS,1,8,1);
    OLED_ShowChar(55,0,'S',8,1);

    //显示当前电芯电压
    OLED_ShowNum(35,13,3,1,24,1);//个位
    OLED_DrawCircle(47,32,1);//小数点
    OLED_ShowNum(50,13,82,2,24,1);//十&百分位
    
    //显示总电压
    OLED_ShowString(90,0,"Total",8,1);
    OLED_ShowNum(90,13,24,2,8,1);//十&个位

    OLED_Refresh();
}

//按键切换UI
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == Button_GPIO_Port)
    {

    }
}



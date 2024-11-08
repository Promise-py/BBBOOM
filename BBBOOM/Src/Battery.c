
#include "Battery.h"
#include "ADS1015.h"

BATTERY Battery[3]={0};
unsigned int ADC1_value[8];//IN 8-14
unsigned int ADC2_value[8];//IN 0-7

/**
 * @brief 初始化电池数据
 * 
 * @param  battery 电池数据结构体指针
 * @param  alarm_voltage 报警电压
 * @param  cut_off_voltage 断电电压
 * 
*/
void Battery_Init(BATTERY *battery,float alarm_voltage,float cut_off_voltage)
{
    uint8_t i=0;
    for ( i = 0; i < 3; i++)
    {
        battery[i].Alarm_Voltage = alarm_voltage;
        battery[i].CutOff_Voltage = cut_off_voltage;
    }
     
    Battery_Refresh(battery);
}

/**
 * @brief 更新电池数据
 * 
 * @param  battery 电池数据结构体指针
 * 
 * @retval 返回值：返回值的描述
 */
void Battery_Refresh(BATTERY *battery)
{
    uint8_t i=0;
    HAL_ADCEx_Calibration_Start(&hadc1);
    for(i=0;i<8;i++)
	{
        HAL_ADC_Start(&hadc1);//启动ADC转换，必须放在for循环中，否则只能采集第一个通道的ADC值;
        HAL_ADC_PollForConversion(&hadc1,100); //用于轮询转换，是一个阻塞函数，等待转换完成，参数100ms是阻塞时间
        if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1),HAL_ADC_STATE_REG_EOC)) //判断转换完成标志位是否设置,为1的时候产生中断。
        ADC1_value[i]=HAL_ADC_GetValue(&hadc1); //开始读ADC的值
	}
    HAL_ADC_Stop(&hadc1); //停止擦剂

    HAL_ADCEx_Calibration_Start(&hadc2);    
    for(i=0;i<8;i++)
	{
        HAL_ADC_Start(&hadc2);//启动ADC转换，必须放在for循环中，否则只能采集第一个通道的ADC值；
        HAL_ADC_PollForConversion(&hadc2,100); //用于轮询转换，是一个阻塞函数，等待转换完成，参数100ms是阻塞时间
        // if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc2),HAL_ADC_STATE_REG_EOC)) //判断转换完成标志位是否设置,为1的时候产生中断。
        ADC2_value[i]=HAL_ADC_GetValue(&hadc2); //开始读ADC的值
	}
    HAL_ADC_Stop(&hadc2); //停止擦剂
    
    //一号电池电芯电压录入
    for ( i = 0; i < 4; i++)
    {
        battery[0].Cells_Voltage[5-i] = (float)ADC1_value[2+i]/4096*3.3*Resistance;
    }

    battery[0].Cells_Voltage[1] = (float)ADC2_value[0]/4096*3.3*Resistance;
    battery[0].Cells_Voltage[0] = (float)ADC2_value[1]/4096*3.3*Resistance;
    

    //二号电池电芯电压录入
    for ( i = 0; i < 6; i++)
    {
        battery[1].Cells_Voltage[5-i] = (float)ADC2_value[2+i]/4095*3.3*Resistance;
    }

    //三号电池电芯电压录入
    battery[2].Cells_Voltage[5] = (float)ADC1_value[6]/4095*3.3*Resistance;
    battery[2].Cells_Voltage[4] = (float)ADC1_value[7]/4095*3.3*Resistance;
    battery[2].Cells_Voltage[3] = (float)ADC1_value[0]/4095*3.3*Resistance;
    battery[2].Cells_Voltage[2] = (float)ADC1_value[1]/4095*3.3*Resistance;

    battery[2].Cells_Voltage[1] = ADS1015_Read(0);
    HAL_Delay(10);
    battery[2].Cells_Voltage[0] = ADS1015_Read(2);

    //电池总电压&电芯节数录入&缺电状态录入
    for ( i = 0; i < 6; i++)
    {
        if(battery[0].Cells_Voltage[i]>1)
        {
            if(battery[0].CutOff_Voltage < battery[0].Cells_Voltage[i]&&battery[0].Cells_Voltage[i]<= battery[0].Alarm_Voltage)battery[0].Battery_State = ALARM;
            else if(battery[0].CutOff_Voltage >= battery[0].Cells_Voltage[i]&&battery[0].Cells_Voltage[i]>3)battery[0].Battery_State = EXHAUST;
            if(battery[0].Cells_Voltage[i]<2 || battery[0].Cells_Voltage[i]>4.3)battery[0].Battery_State = ERR;
            else if(battery[0].Cells_Voltage[i]>4.2)battery[0].Cells_Voltage[i]=4.2;

            battery[0].Battery_Voltage += battery[0].Cells_Voltage[i];
            battery[0].Cells_Num++;
        }

        if(battery[1].Cells_Voltage[i]>1)
        {
            if(battery[1].CutOff_Voltage < battery[1].Cells_Voltage[i]&&battery[1].Cells_Voltage[i]<= battery[1].Alarm_Voltage)battery[1].Battery_State = ALARM;
            else if(battery[1].CutOff_Voltage >= battery[1].Cells_Voltage[i]&&battery[1].Cells_Voltage[i]>3)battery[1].Battery_State = EXHAUST;
            if(battery[1].Cells_Voltage[i]<2 || battery[1].Cells_Voltage[i]>4.3)battery[1].Battery_State = ERR;
            else if(battery[1].Cells_Voltage[i]>4.2)battery[1].Cells_Voltage[i]=4.2;

            battery[1].Battery_Voltage += battery[1].Cells_Voltage[i];
            battery[1].Cells_Num++; 
        }

        if(battery[2].Cells_Voltage[i]>1)
        {
            if(battery[2].CutOff_Voltage < battery[2].Cells_Voltage[i]&&battery[2].Cells_Voltage[i]<= battery[2].Alarm_Voltage)battery[2].Battery_State = ALARM;
            else if(battery[2].CutOff_Voltage >= battery[2].Cells_Voltage[i]&&battery[2].Cells_Voltage[i]>3)battery[2].Battery_State = EXHAUST;
            if(battery[2].Cells_Voltage[i]<2 || battery[2].Cells_Voltage[i]>4.3)battery[2].Battery_State = ERR;
            else if(battery[2].Cells_Voltage[i]>4.2)battery[2].Cells_Voltage[i]=4.2;
            
            battery[2].Battery_Voltage += battery[2].Cells_Voltage[i];
            battery[2].Cells_Num++;
        }
    }

    //电池状态录入
    for ( i = 0; i < 3; i++)
    {
        if(battery[i].Cells_Num==0)battery[i].Battery_State = NUL;
        
        if(battery[i].Battery_State==0)battery[i].Battery_State = READY;
    }
}


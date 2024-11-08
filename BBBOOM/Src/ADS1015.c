#include "ADS1015.h"
#include "main.h"

uint16_t ADS1015_CONFIG;
uint16_t ADS1015_RX_BUF;
void ADS1015_Config(uint8_t i2cAddress,uint8_t channel)
{
    
    // Start with default values
    ADS1015_CONFIG  = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                    ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                    ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1015_REG_CONFIG_DR_3300SPS   | // 1600 samples per second (default)
                    ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)
    
    // Set PGA/voltage range
    ADS1015_CONFIG |= ADS1015_REG_CONFIG_PGA_4_096V;
    switch (channel)
    {
    case (0):
        ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
        break;
    case (1):
        ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
        break;
    case (2):
        ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
        break;
    case (3):
        ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
        break;
    }
    // Set 'start single-conversion' bit
    ADS1015_CONFIG |= ADS1015_REG_CONFIG_OS_SINGLE;

    // Write config register to the ADC
    ADS1015_WriteOneByte(i2cAddress, ADS1015_REG_POINTER_CONFIG, ADS1015_CONFIG);
}


//ADS1015初始化函数
void ADS1015_Init(void)
{
    ADS1015_Config(ADS1015_ADDRESS,0);
    ADS1015_Config(ADS1015_ADDRESS,2);
}

//ADS1015从寄存器读两字节数据
uint16_t ADS1015_ReadOneByte(uint8_t i2cAddress,uint16_t ReadAddr)
{
    uint16_t temp=0;		  	    																 
    HAL_I2C_Master_Transmit(&hi2c2, i2cAddress<<1, &ReadAddr, I2C_MEMADD_SIZE_8BIT,1000);
    HAL_I2C_Mem_Read(&hi2c2, (i2cAddress<<1)+1, ReadAddr, I2C_MEMADD_SIZE_16BIT, &temp, 2,1000);

    // IIC_Start();  
    // IIC_Send_Byte(i2cAddress<<1);  
    // IIC_Wait_Ack();	 
    // IIC_Send_Byte(ReadAddr);  
    // IIC_Wait_Ack();	     	
    // IIC_Stop();	
    
    // IIC_Start();
    // IIC_Send_Byte((i2cAddress<<1)+1);          		   
    // IIC_Wait_Ack();	 
    // temp=IIC_Read_Byte(1);	
    // temp=temp<<8;
    // temp+=IIC_Read_Byte(1);	
    // IIC_Stop();	    
    return temp;
}

//ADS1015写两字节数据
void ADS1015_WriteOneByte(uint8_t i2cAddress,uint16_t WriteAddr,uint16_t DataToWrite)
{
    // uint8_t datH=DataToWrite>>8;
    // uint8_t datL=DataToWrite&0xff;	    																 
    HAL_I2C_Mem_Write(&hi2c2,i2cAddress<<1,WriteAddr,I2C_MEMADD_SIZE_16BIT,&DataToWrite,2,1000);
    
    // HAL_I2C_Transmit(&hi2c2,i2cAddress<<1,WriteAddr,I2C_MEMADD_SIZE_8BIT,&datL,1,0x100);

    // IIC_Send_Byte(WriteAddr);  
    // IIC_Wait_Ack();	     	
    // IIC_Send_Byte(DataToWrite>>8); 
    // IIC_Wait_Ack();	
    // IIC_Send_Byte(DataToWrite&0xff); 
    // IIC_Wait_Ack();	
    // IIC_Stop();
}

//ADS1015读数据
//返回值：获取的AD值
//Channel：通道数，范围0-3
uint16_t ADS1015_Read(uint8_t channel)
{
    int temp;
        // Start with default values
        ADS1015_CONFIG  = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                        ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                        ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                        ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                        ADS1015_REG_CONFIG_DR_3300SPS   | // 1600 samples per second (default)
                        ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)
        
        // Set PGA/voltage range
        ADS1015_CONFIG |= ADS1015_REG_CONFIG_PGA_4_096V;
    if(channel<3) 
    {
        switch (channel)
        {
            case (0):
                ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
            break;
            case (1):
                ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
            break;
            case (2):
                ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
            break;
            case (3):
                ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
            break;
        }	
            // Set 'start single-conversion' bit
        ADS1015_CONFIG |= ADS1015_REG_CONFIG_OS_SINGLE;
        ADS1015_WriteOneByte(ADS1015_ADDRESS,ADS1015_REG_POINTER_CONFIG, ADS1015_CONFIG);
        HAL_Delay(1);
        HAL_I2C_Master_Receive_IT(&hi2c2,ADS1015_ADDRESS<<1,&temp,2);
        temp=temp>>4;
        return temp;
    }
    return 0;
}

//检测AD1015是否存在
//参数：
//i2cAddress：芯片地址
//返回值：0存在 1不存在
uint8_t AD1015_Check(uint8_t i2cAddress)
{
      
    if(HAL_I2C_Master_Transmit(&hi2c2,i2cAddress<<1,0,1,1000)!=HAL_ERROR)
    {
        return 1;
    }  
    else return 0;
}


void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{

    if(hi2c==&hi2c2)
    {
        HAL_I2C_Master_Receive_IT(&hi2c2,ADS1015_ADDRESS<<1,&ADS1015_RX_BUF,2);
        ADS1015_RX_BUF>>4;
    }
    //  i2c_state++;
}



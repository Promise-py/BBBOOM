#include "ADS1015.h"

uint16_t ADS1015_CONFIG;//定义ADS1015配置变量

//ADS1015配置函数
//channel:模数转换通道
void ADS1015_Config(I2C_HandleTypeDef I2cHandle,uint8_t TnputMUX,uint8_t Channel)
{
	// Start with default values
	ADS1015_CONFIG  = ADS1015_REG_CONFIG_CQUE_NONE  | // Disable the comparator (default val)
					ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
					ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
					ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
					ADS1015_REG_CONFIG_DR_3300SPS   | // 3300 samples per second (default)
					ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

	// Set PGA/voltage range
	ADS1015_CONFIG |= ADS1015_REG_CONFIG_PGA_4_096V;
	if(TnputMUX == ADS1015_DUAL_END)
	{
		switch (Channel)
		{
		case (0):
		  ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_DIFF_0_1;
		  break;
		case (1):
		  ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_DIFF_0_3;
		  break;
		case (2):
		  ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_DIFF_1_3;
		  break;
		case (3):
		  ADS1015_CONFIG |= ADS1015_REG_CONFIG_MUX_DIFF_2_3;
		  break;
		}
	}
	else if(TnputMUX == ADS1015_SINGLE_END)
	{
		switch (Channel)
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
	}

	// Set 'start single-conversion' bit
	ADS1015_CONFIG |= ADS1015_REG_CONFIG_OS_SINGLE;
}


//AD1015读取ADC转换数据（即CONVERT寄存器）
//返回值：获取的12位ADC值
int16_t ADS1015_Read_Data(I2C_HandleTypeDef I2cHandle,uint8_t FirADCRead)
{
    int16_t data;
    uint8_t tx_data[1] = {(uint8_t)ADS1015_REG_POINTER_CONVERT};
    uint8_t rx_data[2]={0};

    if(FirADCRead == 1)
    {
		while(HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)(ADS1015_ADDRESS|ADS1015_WRITE_MODE), tx_data, 1, 1000) != HAL_OK)
		{
			if(HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
			{
				
			}
		}
    }

    while(HAL_I2C_Master_Receive(&I2cHandle, (uint16_t)(ADS1015_ADDRESS|ADS1015_READ_MODE), rx_data, 2, 1000) != HAL_OK)
    {
        if(HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
        {
            
        }
    }

    data=rx_data[0]*256U+rx_data[1];
    return data;
}

//AD1015配置寄存器函数（寄存器指针和待配置值的高低比特）
void ADS1015_Config_Register(I2C_HandleTypeDef I2cHandle,uint16_t pointAddr,uint16_t configData)
{
	uint8_t configH,configL;
    configH = configData >> 8;
    configL = configData & 0xFF;
    uint8_t reg_data[3] = {pointAddr,configH,configL};
    while(HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)(ADS1015_ADDRESS|ADS1015_WRITE_MODE), reg_data, 3, 1000) != HAL_OK)
    {
        if(HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
        {
            
        }
    }
}

//检测AD1015是否存在
//返回值：0存在 1不存在
//未找到DataSheet文字记录
uint8_t AD1015_Check(I2C_HandleTypeDef I2cHandle)
{
	uint8_t rx_data[1]={22};
	uint8_t tx_data[1] = {(uint8_t)(ADS1015_ADDRESS<<1)};
	while(HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)(ADS1015_ADDRESS|ADS1015_WRITE_MODE), tx_data, 1, 1000) != HAL_OK)
	{
		if(HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
		{

		}
	}
    while(HAL_I2C_Master_Receive(&I2cHandle, (uint16_t)(ADS1015_ADDRESS|ADS1015_READ_MODE), rx_data, 1, 1000) != HAL_OK)
    {
        if(HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
        {

        }
    }
	return rx_data[0];
}



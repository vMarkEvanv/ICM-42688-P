#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "ICM_42688_P.h"
#include "sys.h"
#include "delay.h"
extern GYRO Gyro_Get;
extern ACC Acc_Get;
extern TEMP Temp;
int main(void)
{
	delay_init();
	OLED_Init();
	OLED_Clear();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	ICM_Port_Init();
	//while(ICM_Gyroscope_Reset()==1);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	ICM_INIT();

	while(1)
	{
		
		GYRO_ACC_TEMP_GET();
		OLED_ShowSignedNum(1, 1, Acc_Get.X, 5);
		OLED_ShowSignedNum(2, 1, Acc_Get.Y, 5);
		OLED_ShowSignedNum(3, 1, Acc_Get.Z, 5);
		OLED_ShowSignedNum(1, 7, Gyro_Get.X, 5);
		OLED_ShowSignedNum(2, 7, Gyro_Get.Y, 5);
		OLED_ShowSignedNum(3, 7, Gyro_Get.Z, 5);
		OLED_ShowSignedNum(4, 1, Temp.T, 5);
		//GPIO_ResetBits(GPIOA,GPIO_Pin_1);

	}
}

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "ICM_42688_P.h"
#include "sys.h"
#include "delay.h"
int main(void)
{
	unsigned char a=0;
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
	Set_Range();
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	
	while(1)
	{
		a = Get_ACC(ACCEL_DATA_Z1);
		OLED_ShowSignedNum(1, 1,a , 5);
		
		//GPIO_ResetBits(GPIOA,GPIO_Pin_1);
		
		
	}
}

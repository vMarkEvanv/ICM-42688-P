#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "ICM_42688_P.h"
#include "sys.h"
#include "delay.h"
int main(void)
{
	delay_init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	Port_Init();
	//GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	while(1)
	{
		
		while(ICM_IIC_WRITE_BYTE());
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
		
		while(1);
	}
}

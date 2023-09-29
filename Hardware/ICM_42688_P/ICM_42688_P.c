#include "ICM_42688_P.h"
#include "stm32f10x.h"                  // Device header
#include "delay.h"
unsigned char ICM_42688_Addr_AD0_LOW_READ = 0xD1;   //AD0低电平地址的读
unsigned char ICM_42688_Addr_AD0_HIGH_READ = 0xD3;	 //AD0高电平地址的读
unsigned char ICM_42688_Addr_AD0_LOW_WRITE = 0xD0;	 //AD0低电平地址的写
unsigned char ICM_42688_Addr_AD0_HIGH_WRITE = 0xD2; //AD0高电平地址的写


void ICM_IIC_Delay(void){
	delay_us(2);
}
/**************************************************/
/*函数名：Port_Init;***********************/
/*功能：初始化硬件IIC通道;************/
/*输入：无;****************************************/
/*输出：无;****************************************/
/**************************************************/
void Port_Init(){
	//delay_init();
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//先使能外设IO PORTB时钟 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	 // 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIO 
	
  GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);						 //PB10,PB11 输出高	

}	  

/************************************************************************/
/*函数名：ICM_42688_START;***********************************************/
/*功能：通过IIC协议连接ICM_42688_P模块;**********************************/
/*输入：无;**************************************************************/
/*输出：0：开始信号发送成功，并且得到应答 1：未得到应答;*****************/
/************************************************************************/
void ICM_42688_START(){
	ICM_SDA_OUT();
	ICM_IIC_SDA=1;	  	  
	ICM_IIC_SCL=1;
	ICM_IIC_Delay();
 	ICM_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	ICM_IIC_Delay();
	ICM_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}

/**************************************************/
/*函数名：ICM_42688_STOP;***********************/
/*功能：停止IIC;************/
/*输入：无;****************************************/
/*输出：无;****************************************/
/**************************************************/
void ICM_42688_STOP(){
	ICM_SDA_OUT();//sda线输出
	ICM_IIC_SCL=0;
	ICM_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	ICM_IIC_Delay();
	ICM_IIC_SCL=1; 
	ICM_IIC_SDA=1;//发送I2C总线结束信号
	ICM_IIC_Delay();							   	
}

/**************************************************/
/*函数名：ICM_IIC_Wait_Ack;************************/
/*功能：等待应答信号到来;**************************/
/*输入：无;****************************************/
/*输出：1，接收应答失败 0，接收应答成功;***********/
/**************************************************/
unsigned char ICM_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	ICM_SDA_IN();      //SDA设置为输入  
	ICM_IIC_SDA=1;ICM_IIC_Delay();	   
	ICM_IIC_SCL=1;ICM_IIC_Delay();	 
	while(ICM_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			ICM_42688_STOP();
			return 1;
		}
	}
	ICM_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 

/**************************************************/
/*函数名：ICM_IIC_Ack;************************/
/*功能：产生ACK应答;**************************/
/*输入：无;****************************************/
/*输出：无;***********/
/**************************************************/
void ICM_IIC_Ack(void)
{
	ICM_IIC_SCL=0;
	ICM_SDA_OUT();
	ICM_IIC_SDA=0;
	ICM_IIC_Delay();
	ICM_IIC_SCL=1;
	ICM_IIC_Delay();
	ICM_IIC_SCL=0;
}

/**************************************************/
/*函数名：ICM_IIC_NAck;************************/
/*功能：不产生ACK应答		;**************************/
/*输入：无;****************************************/
/*输出：无;***********/
/**************************************************/  
void ICM_IIC_NAck(void)
{
	ICM_IIC_SCL=0;
	ICM_SDA_OUT();
	ICM_IIC_SDA=1;
	ICM_IIC_Delay();
	ICM_IIC_SCL=1;
	ICM_IIC_Delay();
	ICM_IIC_SCL=0;
}			

/******************************************************************/
/*函数名：ICM_IIC_Send_Byte;***************************************/
/*功能：IIC发送一个字节,返回从机有无应答;**************************/
/*输入：无;********************************************************/
/*输出：1，有应答 0，无应答;***************************************/
/******************************************************************/  		  
void ICM_IIC_Send_Byte(unsigned char txd)
{                        
    u8 t;   
		ICM_SDA_OUT(); 	    
    ICM_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        ICM_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		    ICM_IIC_SCL=1;
		    ICM_IIC_Delay(); 
		    ICM_IIC_SCL=0;	
		    ICM_IIC_Delay();
    }	 
} 	

/******************************************************************/
/*函数名：ICM_IIC_Read_Byte;***************************************/
/*功能：读1个字节，ack=1时，发送ACK，ack=0，发送nACK ;*************/
/*输入：无;********************************************************/
/*输出：data;***************************************/
/******************************************************************/

unsigned char ICM_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	ICM_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        ICM_IIC_SCL=0; 
        ICM_IIC_Delay();
		ICM_IIC_SCL=1;
        receive<<=1;
        if(ICM_READ_SDA)receive++;   
		ICM_IIC_Delay(); 
    }					 
    if (!ack)
        ICM_IIC_NAck();//发送nACK
    else
        ICM_IIC_Ack(); //发送ACK   
    return receive;
}

/******************************************************************/
/*函数名：ICM_IIC_WRITE_BYTE;***************************************/
/*功能：写一个字节;*************/
/*输入：无;********************************************************/
/*输出：0 失败 1 成功;***************************************/
/******************************************************************/
unsigned char ICM_IIC_WRITE_BYTE(){
	
	ICM_42688_START();
	ICM_IIC_Send_Byte(ICM_42688_Addr_AD0_LOW_WRITE);
	if(ICM_IIC_Wait_Ack()){
		return 1;
	}
	ICM_42688_STOP();
	return 0;
}

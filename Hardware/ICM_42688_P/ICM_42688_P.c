#include "ICM_42688_P.h"
#include "stm32f10x.h"                  // Device header
#include "delay.h"
unsigned char ICM_42688_Addr_AD0_LOW_READ = 0xD1;   //AD0�͵�ƽ��ַ�Ķ�
unsigned char ICM_42688_Addr_AD0_HIGH_READ = 0xD3;	 //AD0�ߵ�ƽ��ַ�Ķ�
unsigned char ICM_42688_Addr_AD0_LOW_WRITE = 0xD0;	 //AD0�͵�ƽ��ַ��д
unsigned char ICM_42688_Addr_AD0_HIGH_WRITE = 0xD2; //AD0�ߵ�ƽ��ַ��д


void ICM_IIC_Delay(void){
	delay_us(2);
}
/**************************************************/
/*��������Port_Init;***********************/
/*���ܣ���ʼ��Ӳ��IICͨ��;************/
/*���룺��;****************************************/
/*�������;****************************************/
/**************************************************/
void Port_Init(){
	//delay_init();
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//��ʹ������IO PORTBʱ�� 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	 // �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIO 
	
  GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);						 //PB10,PB11 �����	

}	  

/************************************************************************/
/*��������ICM_42688_START;***********************************************/
/*���ܣ�ͨ��IICЭ������ICM_42688_Pģ��;**********************************/
/*���룺��;**************************************************************/
/*�����0����ʼ�źŷ��ͳɹ������ҵõ�Ӧ�� 1��δ�õ�Ӧ��;*****************/
/************************************************************************/
void ICM_42688_START(){
	ICM_SDA_OUT();
	ICM_IIC_SDA=1;	  	  
	ICM_IIC_SCL=1;
	ICM_IIC_Delay();
 	ICM_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	ICM_IIC_Delay();
	ICM_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}

/**************************************************/
/*��������ICM_42688_STOP;***********************/
/*���ܣ�ֹͣIIC;************/
/*���룺��;****************************************/
/*�������;****************************************/
/**************************************************/
void ICM_42688_STOP(){
	ICM_SDA_OUT();//sda�����
	ICM_IIC_SCL=0;
	ICM_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	ICM_IIC_Delay();
	ICM_IIC_SCL=1; 
	ICM_IIC_SDA=1;//����I2C���߽����ź�
	ICM_IIC_Delay();							   	
}

/**************************************************/
/*��������ICM_IIC_Wait_Ack;************************/
/*���ܣ��ȴ�Ӧ���źŵ���;**************************/
/*���룺��;****************************************/
/*�����1������Ӧ��ʧ�� 0������Ӧ��ɹ�;***********/
/**************************************************/
unsigned char ICM_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	ICM_SDA_IN();      //SDA����Ϊ����  
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
	ICM_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 

/**************************************************/
/*��������ICM_IIC_Ack;************************/
/*���ܣ�����ACKӦ��;**************************/
/*���룺��;****************************************/
/*�������;***********/
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
/*��������ICM_IIC_NAck;************************/
/*���ܣ�������ACKӦ��		;**************************/
/*���룺��;****************************************/
/*�������;***********/
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
/*��������ICM_IIC_Send_Byte;***************************************/
/*���ܣ�IIC����һ���ֽ�,���شӻ�����Ӧ��;**************************/
/*���룺��;********************************************************/
/*�����1����Ӧ�� 0����Ӧ��;***************************************/
/******************************************************************/  		  
void ICM_IIC_Send_Byte(unsigned char txd)
{                        
    u8 t;   
		ICM_SDA_OUT(); 	    
    ICM_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
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
/*��������ICM_IIC_Read_Byte;***************************************/
/*���ܣ���1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK ;*************/
/*���룺��;********************************************************/
/*�����data;***************************************/
/******************************************************************/

unsigned char ICM_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	ICM_SDA_IN();//SDA����Ϊ����
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
        ICM_IIC_NAck();//����nACK
    else
        ICM_IIC_Ack(); //����ACK   
    return receive;
}

/******************************************************************/
/*��������ICM_IIC_WRITE_BYTE;***************************************/
/*���ܣ�дһ���ֽ�;*************/
/*���룺��;********************************************************/
/*�����0 ʧ�� 1 �ɹ�;***************************************/
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

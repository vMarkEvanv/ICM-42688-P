#ifndef __ICM_42688_P_H
#define __ICM_42688_P_H
#include "sys.h"

//IO方向设置
#define ICM_SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define ICM_SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

//IO操作函数
#define ICM_IIC_SCL    PBout(6) 		//SCL
#define ICM_IIC_SDA    PBout(7) 		//SDA	 
#define ICM_READ_SDA   PBin(7) 		//输入SDA 

/**USER BANK 0 REGISTER MAP – DESCRIPTIONS**/
#define DEVICE_CONFIG 0X11		//Serial IF: R/W	 Reset value: 0x00
#define DRIVE_CONFIG  0X13		//Serial IF: R/W   Reset value: 0x05
#define INT_CONFIG    0X14		//Serial IF: R/W	 Reset value: 0x00
#define FIFO_CONFIG 	0X16		//Serial IF: R/W	 Reset value: 0x00
#define TEMP_DATA1	  0X1D		//Serial IF: SYNCR Reset value: 0x80
#define TEMP_DATA0 		0X1E		//Serial IF: SYNCR Reset value: 0x00
#define ACCEL_DATA_X1 0X1F		//Serial IF: SYNCR Reset value: 0x80
#define ACCEL_DATA_X0 0X20		//Serial IF: SYNCR Reset value: 0x00
#define ACCEL_DATA_Y1 0X21		//Serial IF: SYNCR Reset value: 0x80
#define ACCEL_DATA_Y0 0X22		//Serial IF: SYNCR Reset value: 0x00
#define ACCEL_DATA_Z1 0X23		//Serial IF: SYNCR Reset value: 0x80
#define ACCEL_DATA_Z0 0X24		//Serial IF: SYNCR Reset value: 0x00
#define GYRO_DATA_X1  0X25		//Serial IF: SYNCR Reset value: 0x80
#define GYRO_DATA_X0  0X26		//Serial IF: SYNCR Reset value: 0x00
#define GYRO_DATA_Y1  0X27		//Serial IF: SYNCR Reset value: 0x80
#define GYRO_DATA_Y0  0X28		//Serial IF: SYNCR Reset value: 0x00
#define GYRO_DATA_Z1  0X29		//Serial IF: SYNCR Reset value: 0x80
#define GYRO_DATA_Z0  0X2A		//Serial IF: SYNCR Reset value: 0x00
#define TMST_FSYNCH   0X2B		//Serial IF: SYNCR Reset value: 0x00



void Port_Init(void);
void ICM_42688_START(void);
void ICM_42688_STOP(void);
unsigned char ICM_IIC_Wait_Ack(void);
void ICM_IIC_Ack(void);
void ICM_IIC_NAck(void);
void ICM_IIC_Send_Byte(unsigned char txd);
unsigned char ICM_IIC_Read_Byte(unsigned char ack);
unsigned char ICM_IIC_WRITE_BYTE(void);
#endif 

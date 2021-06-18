/******************** (C) COPYRIGHT 2015 Silicone shell electronic studio **************************
 * �ļ���  ��Lcd1602.c
 * ����    : 1602Һ����ʾ���⺯��        
 * Ӳ�����ӣ� EN:P2.5
 *						RW:P2.6
 *					  RS:P2.7
 *					  D0~D7:P0��
 *
 * ʵ��ƽ̨��SHELL1.0������
 *
 * ����    �����е���
**********************************************************************************/
#include "lcd.h"
#include <intrins.h>
#include <reg52.h>

/*********************LCD1602�˿ڶ���***************************/
sbit LCD_E   			=P2^7;					//ִ��ʹ�ܶ�
sbit LCD_RW  			=P2^6; 				  //��д���ƶ�
sbit LCD_RS  			=P2^5;					//��������ѡ���

/*
 * ��������delay_1s
 * ����  ����ʱ����1s
 *         
 * ����  ��unsigned int time����ʱ��
 * ���  ����
 * ˵��  ��Һ����ʾ����ʱ����
 */
void delay_1s()	  //��ʱ1s
{
	unsigned char i, j, k;

	_nop_();
	i = 8;
	j = 1;
	k = 243;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}		
/*
 * ��������Delay
 * ����  ����ʱ����
 *         
 * ����  ��unsigned int time����ʱ��
 * ���  ����
 * ˵��  ��
 */
void Delay(unsigned int time)
{
    unsigned int timeCounter = 0;
	for (timeCounter = time;timeCounter > 0 ;timeCounter --)
		DelayUs(255);
}
/*
 * ��������DelayUs
 * ����  ����ʱ������΢�룩
 *         
 * ����  ��unsigned int time����ʱ��
 * ���  ����
 * ˵��  ��
 */
void DelayUs(unsigned int time)
{
	unsigned int timeCounter = 0;
	for (timeCounter = 0;timeCounter < time;timeCounter ++)
		_nop_();
}

/****************************************************************************************\
**								   LCD����ģ��								**
\****************************************************************************************/

/*
 * ��������LCDInit
 * ����  ��Һ����ʼ������
 *         
 * ����  ����
 * ���  ����
 * ˵��  ��
 */
void LCDInit(void)
{
    //������ʾģʽ����
	LCD_Data=0;
	LCD_E=Disable;
	Delay(5);
	WriteCmd(LCD_Init);
	Delay(5);
	WriteCmd(LCD_Init);
	Delay(5);
	WriteCmd(LCD_Init);

	WriteCmd(LCD_Init);	   						//��ʼ��
	WriteCmd(LCD_CloseCtr);							//����ʾ
	WriteCmd(LCD_CLS);								//����Ļ
	WriteCmd(LCD_EnterSet);							//����ƶ�����
	WriteCmd(LCD_DispCtr);							//��ʾ���Լ��������
	
}

/*---------------------------------  LCDģʽ���ú���--------------------------------------*\
	
	SetWriteCmd() 	 			����LCDΪд����ģʽ
	SetReadCmd()   		    		����LCDΪ������ģʽ
	SetWriteData()				����LCDΪд����ģʽ
	
\*----------------------------------------------------------------------------------------*/
/*
 * ��������SetWriteCmd
 * ����  ������LCDΪд����ģʽ
 *         
 * ����  ����
 * ���  ����
 * ˵��  ��
 */
void SetWriteCmd(void)
{
	LCD_RW=Write;
	LCD_RS=Cmd;
}
/*
 * ��������SetReadCmd
 * ����  ������LCDΪ������ģʽ
 *         
 * ����  ����
 * ���  ����
 * ˵��  ��
 */
void SetReadCmd(void)
{
	LCD_RW=Read;
	LCD_RS=Cmd;
}
/*
 * ��������SetWriteData
 * ����  ������LCDΪд����ģʽ
 *         
 * ����  ����
 * ���  ����
 * ˵��  ��
 */
void SetWriteData(void)
{
	LCD_RW=Write;
	LCD_RS=Data;
}

/*---------------------------------  LCD����ִ�к���--------------------------------------*\
	
	WriteCmd()					д����
	WriteData()					д����
	ExecuteCmd()					ִ������
	SetXY()						��ʾ��λ
	DisplaySingleChar()			��ʾ�����ַ�
	DisplayString()				��ʾһ���ַ�
	IsBusy()						æ��־���
\*----------------------------------------------------------------------------------------*/
/*
 * ��������WriteCmd
 * ����  ��д����
 *         
 * ����  ����
 * ���  ����
 * ˵��  ��
 */
void WriteCmd(char cmd)
{
	while(IsBusy());
	LCD_Data=cmd;	
	SetWriteCmd();
	ExecuteCmd();
}
/*
 * ��������WriteData
 * ����  ��д����
 *         
 * ����  ����
 * ���  ����
 * ˵��  ��
 */
void WriteData(char ddata)
{
	while(IsBusy());
	LCD_Data=ddata;
	SetWriteData();	
	ExecuteCmd();
}
/*
 * ��������ExecuteCmd
 * ����  ��ִ������
 *         
 * ����  ����
 * ���  ����
 * ˵��  ��
 */
void ExecuteCmd(void)
{
	LCD_E=Enable;
	LCD_E=Disable;
}
/*
 * ��������SetXY
 * ����  ����ʾ��λ
 *         
 * ����  ����
 * ���  ����
 * ˵��  ��
 */
void SetXY(char x,char y)
{
	if (y)
	x|=0x40;
	x|=0x80;
	Delay(5);
	WriteCmd(x);

}
/*
 * ��������DisplaySingleChar
 * ����  ����ʾ�����ַ�
 *         
 * ����  ����
 * ���  ����
 * ˵��  ��
 */
void DisplaySingleChar(char x,char y,char cchar)
{
	SetXY(x,y);
	WriteData(cchar);	
}
/*
 * ��������DisplayString
 * ����  ����ʾ�ַ���
 *         
 * ����  ����
 * ���  ����
 * ˵��  ��
 */
void DisplayString(char x,char y,char *str)
{
	while(*str)
	{
		Delay(5);
		DisplaySingleChar(x++,y,*str);
		str++;
	}
}
/*
 * ��������IsBusy
 * ����  ��æ��־���
 *         
 * ����  ����
 * ���  ����
 * ˵��  ��
 */
bit IsBusy(void)
{
	LCD_Data=0xFF;
	SetReadCmd();
	ExecuteCmd();
//	return (bit)(P0 & 0x80);	
		return (bit)(LCD_Data & 0x80);	
}
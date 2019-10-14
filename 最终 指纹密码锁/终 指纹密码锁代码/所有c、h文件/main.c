#include<reg52.h>
#include<intrins.h>
#include"buzz.h"
#include"key.h"
#include"uart.h"
#include"delay.h"
#include"FPM10A.h"
#include"lcd_1602.h"
#include"kaisuo.h"
#include"i2c.h"

unsigned int w=0;//������λ����ĳ�ʼֵ
unsigned int x=0;
unsigned int y=0;
unsigned int z=0;
unsigned int find_fingerid;


/***ȷ�����뺯��***/
void confirmpassword()
{
	if(w==1&&x==2&&y==3&&z==4)
	{
    LCD1602_Display(0xc0,"  Suceeded ^_^  ",0,16);
		Delay_Ms(1000);
		Buzz_Times(1);
	  kaisuo();    
	}
	else
  {
    LCD1602_Display(0xc0,"  Failed QAQ    ",0,16);
		Delay_Ms(1000);
		Buzz_Times(3);
  }
	x=0;
	y=0;
	z=0;
	w=0;
}


/****���뵽10��0����****/
void change()
{
	if(w>9)
	{
		w-=10;
	}
	if(x>9)
	{
		x-=10;
	}
	if(y>9)
	{
		y-=10;
	}
	if(z>9)
	{
		z-=10;
	}	
}

/****��1.1����ĺ������������������****/
 void display11(void)
 {
	 LCD1602_WriteCMD(0x01);  //����
	 change();
	 LCD1602_Display(0x80,"   Input Code   ",0,16);	//Input Code
	 LCD1602_WriteCMD(0xc0+6);
	 LCD1602_WriteDAT(w+'0');
	 LCD1602_WriteDAT(x+'0');
	 LCD1602_WriteDAT(y+'0');
   LCD1602_WriteDAT(z+'0');
 }
 
 
 /****��1.3����ĺ������������Աģʽ������������棩****/
 void display13(void)
 { 
	 LCD1602_WriteCMD(0x01);  //����
	 change();
	 LCD1602_Display(0x80,"  Input ADCode  ",0,16);	//Input Code
	 LCD1602_WriteCMD(0xc0+6);
	 LCD1602_WriteDAT(w+'0');
	 LCD1602_WriteDAT(x+'0');
	 LCD1602_WriteDAT(y+'0');
	 LCD1602_WriteDAT(z+'0');

 }

 /****��1.3.1����ĺ���������Ա������ȷ��ѡ�����/ɾ��ָ�ƣ�****/
 void display131(void)
 {
	 LCD1602_WriteCMD(0x01);  //���� 	
	 LCD1602_Display(0x80,"  ADMINISTRATOR ",0,16);	 //��һ����ʾ����ָ��
	 LCD1602_Display(0xc0,"  Add     Delete",0,16);	 //��Ӻ�ɾ��ָ��
 }



 
 //�Լ���main����
void main()
{	
	LCD1602_Init();			//��ʼ��Һ��
	LCD1602_Display(0x80,"     By XHJ     ",0,16);	 //Һ��������ʾ����
  Delay_Ms(500);
  Uart_Init();			//��ʼ������
	Key_Init();				//��ʼ������
 	Delay_Ms(500);          //��ʱ500MS���ȴ�ָ��ģ�鸴λ
	Device_Check();		   	//У��ָ��ģ���Ƿ������ȷ��Һ��������Ӧ����ʾ//
	Delay_Ms(1000);			//�Խӳɹ�����ͣ��һ��ʱ��
	Buzz_Times(2);
	
	/******������Ҫҳ��*****/
	begin:
	LCD1602_Display(0x80,"    WELCOME     ",0,16);  //��һ����ʾwelcome
	LCD1602_Display(0xc0,"1.P 2.F 3.A 4.C  ",0,16);	 //1.Password 2.Finger 3.Administrator
	x=0; //��ʼ������
	y=0;
	z=0;
	w=0;
	
	
while(1)
{
//��һ�������������������
	if(KEY1 == 0)
	{	
		Delay_Ms(10);//�ӳ�10ms���չ�������   
    if (KEY1==0)        //�ж��Ƿ񻹰���
		{ 
			Delay_Ms(200);
			display11();//ִ������
		}
		while(1)
		{ 
			if(KEY1==0)
			{	
				Delay_Ms(10);
				if(KEY1==0)
				{
					w+=1;
					Delay_Ms(200);
					display11(); //��ʾ��1.1������
			  }
			}
			if(KEY2==0)
			{
				Delay_Ms(10);
				if(KEY2==0)
				{
					x+=1;				
					Delay_Ms(200);
					display11();
				}
			}
			if(KEY3==0)
			{		
				Delay_Ms(10);				
				if(KEY3==0)
				{
					y+=1;
					Delay_Ms(200);
					display11();
				}
			}
			if(KEY4==0)
			{			
				Delay_Ms(10);
				if(KEY4==0)
				{
					z+=1;
					Delay_Ms(200);
					display11();
				}
			}
			if(KEYOK==0)
			{				
				Delay_Ms(10);
				if(KEYOK==0)
				{
					confirmpassword();	//ȷ������
					goto begin;         //�ص���ʼ����
				}
			}
			if(KEYCANCEL==0)
			{				
				Delay_Ms(10);
				if(KEYCANCEL==0)				
				goto begin;
			}
	  }
  }

//�ڶ���������ָ�ƽ�������
	if(KEY2 == 0)
	{	
		Delay_Ms(10);
		if(KEY2==0)
		{
			Delay_Ms(200);
			while(1)
			{
				if(KEYCANCEL==0)
				{
					Delay_Ms(10);
					if(KEYCANCEL==0)
					{
					  goto begin;
					}
				}
				else
					FPM10A_Find_Fingerprint();	//ʶ��ָ�ƣ����а�����ҳ����ת�Ϳ���������
			}
		}
	}


//������������Ա����
	if(KEY3 == 0)
	{
		Delay_Ms(10);
		if(KEY3==0)
		{
			Delay_Ms(200);
			AD:             //������һ����ʶ�㣬����cancelֱ��������ҳ��
			w=0;
			x=0;
			y=0;
			z=0;
			display13();			//��ʾ��1.3���棨�������Ա���룩
		while(1)
		{ 
			if(KEY1==0)
			{			
				Delay_Ms(10);
				if(KEY1==0)
				{
					w+=1;
					Delay_Ms(200);
					display13();
				}
			}
			if(KEY2==0)
			{			
				Delay_Ms(10);
				if(KEY2==0)
				{
					x+=1;
					Delay_Ms(200);
					display13();
				}
			}
			if(KEY3==0)
			{			
				Delay_Ms(10);
				if(KEY3==0)
				{
					y+=1;
					Delay_Ms(200);
					display13();
				}
			}
			if(KEY4==0)
			{			
				Delay_Ms(10);
				if(KEY4==0)
				{
					z+=1;
					Delay_Ms(200);
					display13();
				}
			}
			if(KEYOK==0) //����1.3.1���棨����Ա���棩
			{			
				Delay_Ms(10);
				if(KEYOK==0)
				{
					if(w==4&&x==3&&y==2&&z==1)
					{
						LCD1602_Display(0xc0,"  Suceeded ^_^  ",0,16);
						Delay_Ms(1500);
					 	LCD1602_WriteCMD(0x01);		
					  display131();         //�ص�1.3.1����
						while(1)
	          {   		     								        
							if(local_date==0)                       //local date��Ϊ��Ļ�� ��*�� λ�õ�ѡ�����
							{
								LCD1602_Display(0x80,  "  ",0,2);
								LCD1602_Display(0xc0,  " *",0,2);
								LCD1602_Display(0xc0+8,"  ",0,2);		
		          }
		          else if(local_date==1)
		          {
								LCD1602_Display(0x80,  "  ",0,2);
								LCD1602_Display(0xc0,  "  ",0,2);
								LCD1602_Display(0xc0+8," *",0,2);		
		          }			
		        //ȷ�ϼ�
		          if(KEYOK == 0)
		          {	 
								while(KEYOK == 0);//�ȴ��ɿ�����								
								switch(local_date)
								{																																					
									case 0:	 //���ָ��
									FPM10A_Add_Fingerprint();     //���ָ�ƣ������ɹ���ʧ����������Ľ�����ʾ��
									goto AD; 									
									case 1:	//���ָ��
									FPM10A_Delete_All_Fingerprint();//ɾ��ָ�ƣ�����ȷ��ɾ�����ɹ���ʧ�ܵĽ�����ʾ��
									goto AD; //�ص�����Ա���棨����ֱ�ӷ��س�ʼ���棩
								}
							}
		    //�л���(�л���*�� ��λ��)
							if(KEY1 == 0)
							{
								while(KEY1 == 0); //�ȴ��ɿ�����				
								if(local_date<=1)
								{
									local_date++;
									if(local_date==2) //ֻ��0��1�����������2����
										local_date=0;						
								}		
			        }	
							if(KEYCANCEL==0)
							{			
								Delay_Ms(10);
								if(KEYCANCEL==0)
									goto begin;    //�ص���ʼ����
							}
							Delay_Ms(100); //��ʱ�ж�100MS���һ��		
						}
					}
			else
			{
				LCD1602_Display(0xc0,"  Failed QAQ    ",0,16);
		    Delay_Ms(1500);					
      }
			x=0;
			y=0;
			z=0;
			w=0;
      goto begin; 
		}
	}		
			if(KEYCANCEL==0)
			{					
				Delay_Ms(10);
				if(KEYCANCEL==0)
				goto begin;
			}
		}
   }
 }
/***���ĸ�����(��ѯ��һ����ָ�ƽ�����ָ��id��)***/	
	if(KEY4==0)
	{
		Delay_Ms(10);
		if(KEY4==0)
		{
			Delay_Ms(200);
			LCD1602_WriteCMD(0x01);
			LCD1602_Display(0x80,"      CHECK     ",0,16);
		}
		while(1)
		{
			if(KEYOK==0)
			{
				Delay_Ms(10);
			  if(KEYOK==0)
				{
					find_fingerid=At24c02Read(1); //��24C02�ж�ȡID��
					LCD1602_WriteCMD(0xc0+7);
		  		LCD1602_WriteDAT(find_fingerid/100+48);
				  LCD1602_WriteDAT(find_fingerid%100/10+48);
				  LCD1602_WriteDAT(find_fingerid%100%10+48);	
				}
			}
			if(KEYCANCEL==0)
			{
				Delay_Ms(10);
				if(KEYCANCEL==0)
					goto begin;
			}
		}
	}
}
}
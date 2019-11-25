#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "sram.h"   
#include "malloc.h" 
#include "usmart.h"  
#include "sdio_sdcard.h"    
#include "malloc.h" 
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	
#include "timer.h"
//bad apple 90fpsˢ������δ���еײ��Ż���ֻ������һЩС���ɣ��ײ��Ż��������ٶȸ���

//���������ϵqq362276794

//������ֻ����STM32F407̽����2.8���������У���������Ҫ�����޸ĳ�������ºϳ�bin�����ļ�

//������ʹ�ö�ʱ��3����ˢ�����٣����ڴ�ӡˢ���ٶȣ����ڲ�����Ϊ115200

//ȫ������ʱ��ע�͵����ٺ�������ȻӰ��ˢ���ٶ�

//��������Ķ����Ʋ����ļ��ŵ��洢��appleĿ¼�£������ļ���Ϊapple.bin������·��Ϊ/apple/apple.bin

//������ֻ������밴��λ������ʱ��SD����ʼ�����ɹ�����
int main(void)
{
	u8 *tempbuf;
	FIL * fftemp;
	u32 bread;
	u32 len_num;
	u32 mm_num;
	u32 num;
	u8 dt;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED  
 	LCD_Init();					//LCD��ʼ��  
 	KEY_Init();					//������ʼ��  
	W25QXX_Init();				//��ʼ��W25Q128
	usmart_dev.init(168);		//��ʼ��USMART
	
	
	TIM3_Int_Init(9999,8399);//��ʼ�������ʱ�����ڲ���ʹ��
	TIM_Cmd(TIM3,DISABLE); 
	
	FSMC_SRAM_Init();			//��ʼ���ⲿSRAM  
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	my_mem_init(SRAMEX);		//��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ�� 
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
  f_mount(fs[0],"0:",1); 		//����SD�� 
 	f_mount(fs[1],"1:",1); 		//����FLASH.
	delay_ms(10);
	font_init();
	delay_ms(10);
	SD_Init();
	delay_ms(10);
	mm_num=9600; //����һ����Ļ��Ҫ���ڴ�
	while(1)
	{
		tempbuf=mymalloc(SRAMEX,mm_num);
		fftemp=(FIL*)mymalloc(SRAMEX,sizeof(FIL));
		f_open (fftemp,"0:/apple/apple.bin", FA_READ); //��·�����ļ�
		LCD_Scan_Dir(L2R_U2D);//������,���ϵ���
		LCD_Set_Window(0,0,320,240);
		LCD_SetCursor(0,0);//���ù��λ�� 
		LCD_WriteRAM_Prepare();   	//��ʼд��GRAM
		len_num=mm_num;			//д������ݳ���
		bread=mm_num;
		
//����ע�͵�5�д��룬�ǲ��ٺ����������Ҫ���٣����ע�ͽ�������ô��ڲ鿴ˢ���ٶȣ����ڲ�����Ϊ115200		

		while(bread==mm_num)//��ѭ��ִ�ж�SD��
		{
//			TIM_Cmd(TIM3,ENABLE);
//			TIM3->CNT=0;
			f_read(fftemp,tempbuf,mm_num,(UINT *)&bread);
			for(len_num=0;len_num<9600;len_num++)
			{
				dt=tempbuf[len_num];
				for(num=0;num<8;num++)
				{
					if((dt<<num)&0x80) LCD->LCD_RAM=0x0000;
					else LCD->LCD_RAM=0xFFFF;
				}
			}
//			num=TIM_GetCounter(TIM3);
//			printf("ˢ���ٶ�:%d FPS\r\n",10000/num);
//			TIM_Cmd(TIM3,DISABLE);
		}
		while(1); //�����꣬while��ѭ��ͣס����
	} 
}








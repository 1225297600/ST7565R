#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "drv_st7565r.h"
#include "drv_lattice_gui.h"

DRV_ST7565R st7565r;
DRV_LATTICE_GUI lat_gui;
char gui_print_buff[128];

/**********************************************************  **********************************************************/
static void DISPLAY_OledPoint(uint16_t x, uint16_t y, bool on);
static void DISPLAY_OledUpload(void);

//static void DISPLAY_GuiTest(void);
//static void DISPLAY_OledTest(void);

/**********************************************************  **********************************************************/
void DISPLAY_Init(DISPLAY_INIT init){
	memmove(&display.init, &init, sizeof(DISPLAY_INIT));
	
	st7565r.hal.delay_ms = BOARD_DELAY_Ms;//delay ms function ��ʱ���뺯��
	st7565r.hal.iic_receive = BOARD_IIC2_Recieve;//IIC recieve function IIC���պ���
	st7565r.hal.iic_transmit = BOARD_IIC2_Transmit;//IIC transmit function IIC���ͺ���
	st7565r.set.addr = 0x78;//IIC address IIC�豸��ַ
	st7565r.set.w = 128;//screen width ��Ļ���
	st7565r.set.h = 64;//screen height ��Ļ�߶�
	st7565r.set.dx = 2;//screen width offset ��Ļ����ƫ��
	st7565r.set.dy = 0;//screen height offset ��Ļ����ƫ��
	DRV_ST7565R_Init(&st7565r, 0x00);//screen Driver Init ��Ļ������ʼ��
	
	lat_gui.hal.DrawPoint = DISPLAY_OledPoint;//GUI draw point interface  GUI����һ����Ľӿ�
	lat_gui.hal.DrawUpload = DISPLAY_OledUpload;//GUI all draw interface  GUIȫ��ˢ�µĽӿ�
	lat_gui.print.buff = gui_print_buff;//GUI cache buff GUI����
	lat_gui.print.len = 128;//GUI cache size GUI�����С
	
//	DISPLAY_OledTest();//screen drv basec test ��Ļ��������
//	DISPLAY_GuiTest();//GUI interface test GUI�ӿڲ���
	
}

DISPLAY DISPLAY_Get(void){
	return display;
}

void DISPLAY_Poller(void){
	DRV_LATTICE_GUI_Print(&lat_gui, GUI_FONTS_W6H8, 0,0, "Time:%d    ", BOARD_SYS_MsTick()/1000);
	DRV_LATTICE_GUI_Upload(&lat_gui);
}

/**********************************************************  **********************************************************/
static void DISPLAY_OledPoint(uint16_t x, uint16_t y, bool on){
	if(x < st7565r.set.w){
		if(y < st7565r.set.h){
			DRV_ST7565R_Point(&st7565r, x,y,on);
		}
	}
}
static void DISPLAY_OledUpload(void){
	DRV_ST7565R_Refresh(&st7565r);
}

///********************************************************** Test **********************************************************/
//static uint8_t img_connected[]={
//	//MSB  ���x�߶�=13x8  ˮƽɨ��
//	0x0D,0x00,0x6C,0x03,0x79,0xDB,0xFF,0xDB,0xBE,0xC0,0x36,0x00,0xB0
//};

//static void DISPLAY_GuiTest(void){
//	//2��1��
//	DRV_LATTICE_GUI_Line(&lat_gui, 0,63, 127,0, true);
//	DRV_LATTICE_GUI_Line(&lat_gui, 0,0, 127,63, true);
//	DRV_LATTICE_GUI_Point(&lat_gui, 0,32, true);
//	
//	//���һ��
//	DRV_LATTICE_GUI_Fill(&lat_gui, 100,32, 5,5,  true);
//	
//	//��ʾͼƬ
//	DRV_LATTICE_GUI_Imag(&lat_gui, 10,32, 13,8, img_connected, true, false);
//	
//	//�ַ�
//	DRV_LATTICE_GUI_Print(&lat_gui, GUI_FONTS_W6H8, 50,0, "test");
//	
//	//��Բ
//	DRV_LATTICE_GUI_Round(&lat_gui, 64,32, 16,  true);
//	
//	//����
//	DRV_LATTICE_GUI_Rectangle(&lat_gui, 40,10, 48,44,  true);
//	
//	DRV_LATTICE_GUI_Upload(&lat_gui);
//}

//static void DISPLAY_OledTest(void){
//	DRV_ST7565R_Point(&st7565r, 0,0, true);
//	DRV_ST7565R_Point(&st7565r, 0,63, true);
//	DRV_ST7565R_Point(&st7565r, 127,0, true);
//	DRV_ST7565R_Point(&st7565r, 127,63, true);
//	
//	//�������γ�һ��7��
//	DRV_ST7565R_Point(&st7565r, 65,32, true);//���Ͻ�
//	DRV_ST7565R_Point(&st7565r, 65,33, true);//���½�
//	DRV_ST7565R_Point(&st7565r, 64,32, true);//���Ͻ�
//	
//	DRV_ST7565R_Refresh(&st7565r);
//}


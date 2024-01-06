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
	
	DRV_ST7565R_Init(&st7565r, 0x00, 0x78, 128,64, 2,0, BOARD_DelayMs, IMPT_I2C_Receive, IMPT_I2C_Transmit);//screen Driver Init
	DRV_LATTICE_GUI_Init(&lat_gui, gui_print_buff, 128, DISPLAY_OledPoint, DISPLAY_OledUpload);
	
//	DISPLAY_OledTest();//screen drv basec test 屏幕驱动测试
//	DISPLAY_GuiTest();//GUI interface test GUI接口测试
	
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
//	//MSB  宽度x高度=13x8  水平扫描
//	0x0D,0x00,0x6C,0x03,0x79,0xDB,0xFF,0xDB,0xBE,0xC0,0x36,0x00,0xB0
//};

//static void DISPLAY_GuiTest(void){
//	//2线1点
//	DRV_LATTICE_GUI_Line(&lat_gui, 0,63, 127,0, true);
//	DRV_LATTICE_GUI_Line(&lat_gui, 0,0, 127,63, true);
//	DRV_LATTICE_GUI_Point(&lat_gui, 0,32, true);
//	
//	//填充一块
//	DRV_LATTICE_GUI_Fill(&lat_gui, 100,32, 5,5,  true);
//	
//	//显示图片
//	DRV_LATTICE_GUI_Imag(&lat_gui, 10,32, 13,8, img_connected, true, false);
//	
//	//字符
//	DRV_LATTICE_GUI_Print(&lat_gui, GUI_FONTS_W6H8, 50,0, "test");
//	
//	//画圆
//	DRV_LATTICE_GUI_Round(&lat_gui, 64,32, 16,  true);
//	
//	//矩形
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
//	//三个点形成一个7形
//	DRV_ST7565R_Point(&st7565r, 65,32, true);//右上角
//	DRV_ST7565R_Point(&st7565r, 65,33, true);//右下角
//	DRV_ST7565R_Point(&st7565r, 64,32, true);//左上角
//	
//	DRV_ST7565R_Refresh(&st7565r);
//}


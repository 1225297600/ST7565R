#ifndef __DRV_LATTICE_GUI_H
#define __DRV_LATTICE_GUI_H
#include <stdint.h>
#include <stdbool.h>

/*
	xyÄ¬ÈÏÊ¹ÓÃ£¬Ç÷ÓÚ0µÄ·½Ïò
*/

typedef struct{
	struct{
		char *buff;
		uint16_t len;
	}print;
	
	struct{
		void (*drawPoint)(uint16_t x, uint16_t y, bool on);
		void (*drawUpload)(void);
	}hal;
}DRV_LATTICE_GUI;

typedef enum{
	GUI_FONTS_W6H8 = 0,
	GUI_FONTS_W8H16,
}DRV_LATTICE_GUI_FONTS;

void DRV_LATTICE_GUI_Init(DRV_LATTICE_GUI *gui, 
		char *buff, uint16_t len,
		void (*drawPoint)(uint16_t x, uint16_t y, bool on),
		void (*drawUpload)(void));
void DRV_LATTICE_GUI_Upload(DRV_LATTICE_GUI *gui);

void DRV_LATTICE_GUI_Point(DRV_LATTICE_GUI *gui, uint16_t x, uint16_t y, bool on);
void DRV_LATTICE_GUI_Line(DRV_LATTICE_GUI *gui, uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, bool on);
void DRV_LATTICE_GUI_Fill(DRV_LATTICE_GUI *gui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool on);
//horiz=true Ë®Æ½É¨Ãè£¬lsbÐ¡¶ËÄ£Ê½£¨µÍµÍ£©
void DRV_LATTICE_GUI_Imag(DRV_LATTICE_GUI *gui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *img, bool horiz, bool lsb);
void DRV_LATTICE_GUI_Print(DRV_LATTICE_GUI *gui, DRV_LATTICE_GUI_FONTS fonts, uint16_t x, uint16_t y, char* fmt,...);
void DRV_LATTICE_GUI_Round(DRV_LATTICE_GUI *gui, uint16_t center_x, uint16_t center_y, uint16_t r, bool on);
void DRV_LATTICE_GUI_Rectangle(DRV_LATTICE_GUI *gui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool on);

#endif

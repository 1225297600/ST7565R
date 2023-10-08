#include "drv_lattice_gui.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include "drv_fonts.h"

static void DRV_GUI_DrawPoint(DRV_LATTICE_GUI *gui, uint16_t x, uint16_t y, bool on){
	if(gui->hal.DrawPoint != NULL){
		gui->hal.DrawPoint(x,y,on);
	}else{
		
	}
}

static void DRV_GUI_DrawUpload(DRV_LATTICE_GUI *gui){
	if(gui->hal.DrawUpload != NULL){
		gui->hal.DrawUpload();
	}else{
		
	}
}

/**********************************************************  **********************************************************/

void DRV_LATTICE_GUI_Upload(DRV_LATTICE_GUI *gui){
	DRV_GUI_DrawUpload(gui);
}


void DRV_LATTICE_GUI_Point(DRV_LATTICE_GUI *gui, uint16_t x, uint16_t y, bool on){
	DRV_GUI_DrawPoint(gui, x,y,on);
}

void DRV_LATTICE_GUI_Line(DRV_LATTICE_GUI *gui, uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, bool on){
	int x, y;
	float k, b;
	
	//横线
	if(end_y == start_y){
		if(start_x > end_x){
			//交换方向
			x = end_x;
			end_x = start_x;
			start_x = x;
		}
		for(x=start_x, y=start_y; x<end_x; x++){
			DRV_GUI_DrawPoint(gui, x,y,on);
		}
		return;
	}
	
	//竖线
	if(end_x == start_x){
		if(start_y > end_y){
			//交换方向
			y = end_y;
			end_y = start_y;
			start_y = y;
		}
		for(y=start_y, x=start_x; y<end_y; y++){
			DRV_GUI_DrawPoint(gui, x,y,on);
		}
		return;
	}
	
	//非竖线和横线
	k = (end_y - start_y);
	k /= (end_x - start_x);
	b = start_y;
	b -= k*start_x;
	x = start_x;
	for(; ; ){
		y = k*x+b;
		if(y < 0){
			break;
		}
		
		DRV_GUI_DrawPoint(gui, x,y,on);
		
		if(start_x < end_x){
			 x++;
			if(x > end_x){
				break;
			}
		}else{
			x--;
			if(x < end_x){
				break;
			}
		}
	}
	
}

void DRV_LATTICE_GUI_Fill(DRV_LATTICE_GUI *gui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool on){
	for(uint16_t dx=0; dx<w; dx++){
		for(uint16_t dy=0; dy<h; dy++){
			DRV_GUI_DrawPoint(gui, dx+x,dy+y,on);
		}
	}
}

void DRV_LATTICE_GUI_Imag(DRV_LATTICE_GUI *gui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *img, bool horiz, bool lsb){
	uint16_t dbyte, dbit, dx, dy;
	uint8_t check_byte;
	bool on;
	//horiz=true 水平扫描，lsb小端模式（低低）
	const uint16_t bit = w*h;
	
	for(uint16_t i=0; i<bit; i++){
		//
		dbyte = i/8;
		dbit = i%8;
		
		//
		if(horiz){
			dx = i%w;
			dy = i/w;
		}else{
			dx = i/h;
			dy = i%h;
		}
		
		//
		if(lsb){
			check_byte = 1 << dbit;
		}else{
			check_byte = 0x80 >> dbit;
		}
		
		//
		if((img[dbyte] & check_byte) != 0){
			on = true;
		}else{
			on = false;
		}
		
		//
		DRV_GUI_DrawPoint(gui, x+dx,  y+dy, on);
	}
}



void DRV_LATTICE_GUI_Print(DRV_LATTICE_GUI *gui, DRV_LATTICE_GUI_FONTS fonts, uint16_t x, uint16_t y, char* fmt,...){
	uint16_t len;
	va_list ap;
	
	va_start(ap,fmt);
	if(vsnprintf(gui->print.buff, gui->print.len, fmt, ap) < 0){
		return;
	}
	va_end(ap);
	for(uint16_t i=0; i< strlen(gui->print.buff); i++){
		//[sp]开头 [~]结尾 暂时只做了这些字符
		if((gui->print.buff[i] < ' ') ||
			(gui->print.buff[i] > '~'))
		{
			gui->print.buff[i] = '?';
		}
		
		switch(fonts){
			case GUI_FONTS_W6H8:
				DRV_LATTICE_GUI_Imag(gui, x+6*i,y, 6,8, (uint8_t*)DRV_FONTS_W6H8[gui->print.buff[i]-' '], false, true);
				break;
			case GUI_FONTS_W8H16:
				//这个字体是分上下两块
				DRV_LATTICE_GUI_Imag(gui, x+8*i,y, 8,8, (uint8_t*)DRV_FONTS_W8H16[gui->print.buff[i]-' '], false, true);
				DRV_LATTICE_GUI_Imag(gui, x+8*i,y+8, 8,8, (uint8_t*)&DRV_FONTS_W8H16[gui->print.buff[i]-' '][8], false, true);
				break;
		}
		
	}
}

void DRV_LATTICE_GUI_Round(DRV_LATTICE_GUI *gui, uint16_t center_x, uint16_t center_y, uint16_t r, bool on){
	double x, y;
	
	//(x)^2+(y)^w=r^2 圆心在原点（0，0）
	//(x-center_x)^2+(y-center_y)^w=r^2
	
	//填充x方向
	for(x=-r; x<(r); x++){
		y = sqrt(fabs( pow(r,2) - pow(x,2) ));
		if(((-y + center_y) > 0)
			&& ((y + center_y) < 65536)
			&& ((x + center_x) > 0)
			&& ((x + center_x) < 65536))
		{
			DRV_GUI_DrawPoint(gui, x + center_x, -y + center_y, on);
			DRV_GUI_DrawPoint(gui, x + center_x, y + center_y, on);
		}
	}//for
	
	//填充y方向
	for(y=-r; y<(r); y++){
		x = sqrt(fabs( pow(r,2) - pow(y,2) ));
		if(((y + center_y) > 0)
			&& ((y + center_y) < 65536)
			&& ((-x + center_x) > 0)
			&& ((x + center_x) < 65536))
		{
			DRV_GUI_DrawPoint(gui, -x + center_x, y + center_y, on);
			DRV_GUI_DrawPoint(gui, x + center_x, y + center_y, on);
		}
	}//for
	
}

void DRV_LATTICE_GUI_Rectangle(DRV_LATTICE_GUI *gui, uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool on){
	DRV_LATTICE_GUI_Line(gui, x,y, x,y+h, true);
	DRV_LATTICE_GUI_Line(gui, x,y, x+w,y, true);
	DRV_LATTICE_GUI_Line(gui, x+w,y+h, x,y+h, true);
	DRV_LATTICE_GUI_Line(gui, x+w,y+h, x+w,y, true);
}




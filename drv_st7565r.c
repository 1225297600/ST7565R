#include "drv_st7565r.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

void DRV_ST7565R_Transmit(DRV_ST7565R *st7565r, uint8_t dev, uint8_t *data, uint16_t len)
{
	if(st7565r->hal.iic_transmit != NULL){
		st7565r->hal.iic_transmit(dev, data, len);
	}else{
		
	}
}
void DRV_ST7565R_Receive(DRV_ST7565R *st7565r, uint8_t dev, uint8_t *data, uint16_t len)
{
	if(st7565r->hal.iic_receive != NULL){
		st7565r->hal.iic_receive(dev, data, len);
	}else{
		
	}
}
void DRV_ST7565R_DelayMs(DRV_ST7565R *st7565r, uint32_t ms)
{
	if(st7565r->hal.delay_ms != NULL){
		st7565r->hal.delay_ms(ms);
	}else{
		
	}
}

/**********************************************************  **********************************************************/
static void DRV_ST7565R_WriteByte(DRV_ST7565R *st7565r, uint8_t reg_addr,uint8_t data)
{
	uint8_t send_data[2] = { reg_addr, data};
	DRV_ST7565R_Transmit(st7565r, st7565r->set.addr, send_data, 2);
}

static uint8_t DRV_ST7565R_ReadByte(DRV_ST7565R *st7565r, uint8_t reg_addr)
{
	uint8_t reg;
	DRV_ST7565R_Transmit(st7565r, st7565r->set.addr, &reg_addr, 1);
	DRV_ST7565R_Receive(st7565r, st7565r->set.addr, &reg, 1);
	return reg;
}

/**********************************************************  **********************************************************/

static void DRV_ST7565R_WriteCmd(DRV_ST7565R *st7565r, uint8_t cmd)//写命令
{
	//CMD regAddr 0x00
	DRV_ST7565R_WriteByte(st7565r, 0x00, cmd);
}
static void DRV_ST7565R_WriteData(DRV_ST7565R *st7565r, uint8_t cmd)//写数据
{
	//DATA regAddr 0x40
	DRV_ST7565R_WriteByte(st7565r, 0x40, cmd);
}




static void DRV_ST7565R_SetDisplay(DRV_ST7565R *st7565r, bool on)
{
	uint8_t set = 0;
	if(on)set = 1;
	DRV_ST7565R_WriteCmd(st7565r, 0xAE | set);
}

static void DRV_ST7565R_SetStartLine(DRV_ST7565R *st7565r, uint8_t line)
{
	//[0,63]
	DRV_ST7565R_WriteCmd(st7565r, 0x40 | line);
}

static void DRV_ST7565R_SetPoint(DRV_ST7565R *st7565r, uint8_t x, uint8_t page) //设置起始点坐标
{
	//[0,8]
	DRV_ST7565R_WriteCmd(st7565r, 0xB0 | page);
	//[0,131]
	DRV_ST7565R_WriteCmd(st7565r, 0x10 | ((x >> 4) & 0x0f));
	DRV_ST7565R_WriteCmd(st7565r, 0x00 | (x & 0x0f));
}

static void DRV_ST7565R_SetContrast(DRV_ST7565R *st7565r, float pct)
{
	//[0,0xff]
	if(pct>100)pct=100;
	pct = pct * 0xff / 100;
	uint8_t pct_u8 = pct;
	
	DRV_ST7565R_WriteCmd(st7565r, 0x81);
	DRV_ST7565R_WriteCmd(st7565r, pct_u8);
}

static void DRV_ST7565R_SetReverseLR(DRV_ST7565R *st7565r, bool reverse)
{
	//left right
	uint8_t set = 1;
	if(reverse)set = 0;
	DRV_ST7565R_WriteCmd(st7565r, 0xa0 | set);
}

static void DRV_ST7565R_SetReverseUP(DRV_ST7565R *st7565r, bool reverse)
{
	//up down
	uint8_t set = 8;
	if(reverse)set = 0;
	DRV_ST7565R_WriteCmd(st7565r, 0xc0 | set);
}

static void DRV_ST7565R_SetReversePoint(DRV_ST7565R *st7565r, bool reverse)
{
	uint8_t set = 0;
	if(reverse)set = 1;
	DRV_ST7565R_WriteCmd(st7565r, 0xa6 | set);
}

static void DRV_ST7565R_SetRamDisable(DRV_ST7565R *st7565r, bool disable)
{
	uint8_t set = 0;
	if(disable)set = 1;
	DRV_ST7565R_WriteCmd(st7565r, 0xa4 | set);
}

static void DRV_ST7565R_SetAllPoint(DRV_ST7565R *st7565r, bool force_open)
{
	uint8_t set = 0;
	if(force_open)set = 1;
	DRV_ST7565R_WriteCmd(st7565r, 0xa4 | set);
}

static void DRV_ST7565R_SetOffset(DRV_ST7565R *st7565r, uint8_t offset)
{
	DRV_ST7565R_WriteCmd(st7565r, 0xd3);
	DRV_ST7565R_WriteCmd(st7565r, offset);
}

static void DRV_ST7565R_SetMultiplex(DRV_ST7565R *st7565r, uint8_t multiplex)
{
	DRV_ST7565R_WriteCmd(st7565r, 0xa8);
	DRV_ST7565R_WriteCmd(st7565r, multiplex);
}

static void DRV_ST7565R_SetCycDiv(DRV_ST7565R *st7565r, uint8_t div)
{
	DRV_ST7565R_WriteCmd(st7565r, 0xd5);
	DRV_ST7565R_WriteCmd(st7565r, div);
}

static void DRV_ST7565R_SetChargeCycNum(DRV_ST7565R *st7565r, uint8_t cyc_num)
{
	DRV_ST7565R_WriteCmd(st7565r, 0xd9);
	DRV_ST7565R_WriteCmd(st7565r, cyc_num);
}

static void DRV_ST7565R_SetCom_(DRV_ST7565R *st7565r, uint8_t com_)
{
	DRV_ST7565R_WriteCmd(st7565r, 0xda);
	DRV_ST7565R_WriteCmd(st7565r, com_);
}

static void DRV_ST7565R_SetVcomh(DRV_ST7565R *st7565r, uint8_t vcomh)
{
	DRV_ST7565R_WriteCmd(st7565r, 0xdb);
	DRV_ST7565R_WriteCmd(st7565r, vcomh);
}

static void DRV_ST7565R_SetDcdc(DRV_ST7565R *st7565r, bool on)
{
	uint8_t set = 0x10;
	if(on)set = 0x14;
	DRV_ST7565R_WriteCmd(st7565r, 0x8d);
	DRV_ST7565R_WriteCmd(st7565r, set);
}
/**********************************************************  **********************************************************/

static void DRV_ST7565R_Display(DRV_ST7565R *st7565r)
{
	uint8_t m,n;
	for(m=0;m<ST7565R_PAGE;m++)
	{
		DRV_ST7565R_SetPoint(st7565r, 0, m);
		for(n=0;n<ST7565R_W;n++)
		{
			DRV_ST7565R_WriteData(st7565r, st7565r->status.ram[m][n]);
		}
	}
}

/**********************************************************  **********************************************************/
//初始化OLED
void DRV_ST7565R_Init(DRV_ST7565R *st7565r, uint8_t fill){
	DRV_ST7565R_DelayMs(st7565r, 100);//这里的延时很重要
	
	DRV_ST7565R_SetDisplay(st7565r, false);
		
	DRV_ST7565R_WriteCmd(st7565r,0x20);	//Set Memory Addressing Mode	
	DRV_ST7565R_WriteCmd(st7565r, 0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	
	
	DRV_ST7565R_SetMultiplex(st7565r, 0x3F);//
	DRV_ST7565R_SetRamDisable(st7565r, false);
	DRV_ST7565R_SetOffset(st7565r, 0);
	DRV_ST7565R_SetCycDiv(st7565r, 0xf0);
	DRV_ST7565R_SetChargeCycNum(st7565r, 0x22);
	DRV_ST7565R_SetCom_(st7565r, 0x12);//
	DRV_ST7565R_SetVcomh(st7565r, 0x20);//=Vcc*0.77
	DRV_ST7565R_SetDcdc(st7565r, true);
	
	DRV_ST7565R_SetContrast(st7565r, 50);
	
	DRV_ST7565R_SetStartLine(st7565r, 0);
	DRV_ST7565R_SetPoint(st7565r, 0, 0);
	DRV_ST7565R_SetReverseLR(st7565r, false);
	DRV_ST7565R_SetReverseUP(st7565r, false);
	DRV_ST7565R_SetReversePoint(st7565r, false);
	
	DRV_ST7565R_SetDisplay(st7565r, true);
	
	DRV_ST7565R_DelayMs(st7565r, 100); //这里的延时很重要
	memset(st7565r->status.ram , fill, ST7565R_PAGE*ST7565R_W);
	DRV_ST7565R_Display(st7565r);
}

void DRV_ST7565R_Refresh(DRV_ST7565R *st7565r){
	DRV_ST7565R_Display(st7565r);
}

void DRV_ST7565R_Point(DRV_ST7565R *st7565r, uint8_t x, uint8_t y, bool on){
	//偏移
	x += st7565r->set.dx;
	y += st7565r->set.dy;
	
	//判断
	if(y >= ST7565R_H){
		return;
	}
	if(x >= ST7565R_W){
		return;
	}
	
	//绘制
	if(on){
		//page = (y-y%8)/8
		st7565r->status.ram[y/8][x] |= (1<<y%8);
	}else{
		st7565r->status.ram[y/8][x] &= (~(1<<y%8));
	}
}



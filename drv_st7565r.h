#ifndef __DRV_ST7565R_H
#define __DRV_ST7565R_H
#include <stdint.h>
#include <stdbool.h>

#define ST7565R_W				132
#define ST7565R_H				64
#define ST7565R_PAGE		8

/*
	Addr	:	0x78
	ÊÖ²áÐ´µÄ9Ò³£¬Êµ²âÖ»ÓÐ8Ò³£¬Ð´µÚ9Ò³»á¸²¸ÇµÚ1Ò³
	DRV_ST7565R.status.ram = 65x132£º
	[byte-0 bit-0]	[byte-1 bit-0] ... [byte-16 bit-0] [byte-17 bit-0]
	[byte-0 bit-1]
	[byte-0 bit-2]
	[byte-0 bit-3]
	[byte-0 bit-4]
	[byte-0 bit-5]
	[byte-0 bit-6]
	[byte-0 bit-7]
	
	[byte-1 bit-0]
	...
	[byte-7 bit-6]
	[byte-7 bit-7]
	
	[byte-8 bit-0]
*/

typedef struct{
	struct{
		uint8_t addr;
		uint8_t w;//¿í¶È
		uint8_t h;//¸ß¶È
		uint8_t dx;//ÁÐÆ«ÒÆ
		uint8_t dy;//ÐÐÆ«ÒÆ
	}set;
	
	struct{
		uint8_t ram[ST7565R_PAGE][ST7565R_W];
	}status;
	
	struct{
		void (*delay_ms)(uint32_t ms);
		void (*iic_transmit)(uint8_t dev, uint8_t *data, uint16_t len);
		void (*iic_receive)(uint8_t dev, uint8_t *data, uint16_t len);
	}hal;
}DRV_ST7565R;

void DRV_ST7565R_Init(DRV_ST7565R *st7565r, 
	uint8_t fill,
	uint8_t addr,
	uint8_t w,//¿í¶È
	uint8_t h,//¸ß¶È
	uint8_t dx,//ÁÐÆ«ÒÆ
	uint8_t dy,//ÐÐÆ«ÒÆ
	void (*delay_ms)(uint32_t ms),
	void (*iic_transmit)(uint8_t dev, uint8_t *data, uint16_t len),
	void (*iic_receive)(uint8_t dev, uint8_t *data, uint16_t len));
void DRV_ST7565R_Refresh(DRV_ST7565R *st7565r);
void DRV_ST7565R_Point(DRV_ST7565R *st7565r, uint8_t x, uint8_t y, bool on);

#endif



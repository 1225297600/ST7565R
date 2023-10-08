#ifndef __DRV_ST7565R_H
#define __DRV_ST7565R_H
#include <stdint.h>
#include <stdbool.h>

#define ST7565R_W				132
#define ST7565R_H				64
#define ST7565R_PAGE		8

/*
	Addr	:	0x78
	�ֲ�д��9ҳ��ʵ��ֻ��8ҳ��д��9ҳ�Ḳ�ǵ�1ҳ
	DRV_ST7565R.status.ram = 65x132��
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
		uint8_t w;//���
		uint8_t h;//�߶�
		uint8_t dx;//��ƫ��
		uint8_t dy;//��ƫ��
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

void DRV_ST7565R_Init(DRV_ST7565R *st7565r, uint8_t fill);
void DRV_ST7565R_Refresh(DRV_ST7565R *st7565r);
void DRV_ST7565R_Point(DRV_ST7565R *st7565r, uint8_t x, uint8_t y, bool on);

#endif



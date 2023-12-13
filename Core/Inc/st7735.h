#ifndef INC_ST7735_H_
#define INC_ST7735_H_

#include "main.h"
#include "pin_defines.h"

#define ST7735_WIDTH	160
#define ST7735_HEIGHT	128
#define ST7735_HEIGHT_HALF 64

#define ST7735_OFFSET_X	1
#define ST7735_OFFSET_Y	2

#define VOLTAGE_EDGE_DISTANCE 16
#define VOLTAGE_FRAME_X0	  0
#define VOLTAGE_FRAME_Y0	  0

#define VOLTAGE_FRAME_XL	129
#define VOLTAGE_FRAME_YL	97

#define VOLTAGE_THICKNESS	6

#define ST7735_SLPOUT			0x11
#define ST7735_DISPOFF			0x28
#define ST7735_DISPON			0x29
#define ST7735_CASET			0x2a
#define ST7735_RASET			0x2b
#define ST7735_RAMWR			0x2c
#define ST7735_MADCTL			0x36
#define ST7735_COLMOD			0x3a
#define ST7735_FRMCTR1			0xb1
#define ST7735_FRMCTR2			0xb2
#define ST7735_FRMCTR3			0xb3
#define ST7735_INVCTR			0xb4
#define ST7735_PWCTR1			0xc0
#define ST7735_PWCTR2			0xc1
#define ST7735_PWCTR3			0xc2
#define ST7735_PWCTR4			0xc3
#define ST7735_PWCTR5			0xc4
#define ST7735_VMCTR1			0xc5
#define ST7735_GAMCTRP1		0xe0
#define ST7735_GAMCTRN1		0xe1

#define ST7735_BLACK		0x0000
#define ST7735_RED			0xF800
#define ST7735_GREEN		0x07E0
#define ST7735_BLUE		    0x1F00
#define ST7735_YELLOW		0xFFE0
#define ST7735_MAGENTA		0x1FF8
#define ST7735_CYAN		    0xFF07
#define ST7735_WHITE		0xFFFF
#define ST7735_GRAY		    0x7BEF

#define CMD(x)	((x) | 0x100)


#define ST7735_DATA_TYPE		HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_SET)
#define ST7735_COMMAND_TYPE	    HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_RESET)

#define SPI_START	HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_RESET)
#define SPI_STOP	HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_SET)

void ST7735_Init(void);
void ST7735_Draw_Pixel(uint16_t x, uint16_t y, uint16_t color);
void ST7735_Draw_Rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
void ST7735_Draw_HorizontalLine(uint16_t xo, uint16_t y, uint16_t width, uint16_t color);
void ST7735_Draw_VerticalLine(uint16_t x, uint16_t yo, uint16_t height, uint16_t color);
void ST7735_Draw_FFT(float *fft_values, size_t fft_buffer_size, uint16_t color);
void ST7735_Draw_Voltage(float *voltage_values, size_t voltage_buffer_size, uint16_t color, uint16_t vol_resolution);

#if ST7735_USE_CONTRAST == 1
	void ST7735_SetContrast(uint8_t percent);
#endif

#endif /* INC_ST7735_H_ */


#ifndef INC_PIN_DEFINES_H_
#define INC_PIN_DEFINES_H_

#include "spi.h"

#define st7735_spi hspi2
extern SPI_HandleTypeDef st7735_spi;

#define ST7735_USE_CONTRAST 0

#if ST7735_USE_CONTRAST == 1
	#define st7735_timer         htim4
	#define st7735_timer_channel TIM_CHANNEL_1
	extern TIM_HandleTypeDef st7735_timer;
#endif


#define ST7735_DC_PORT GPIOA
#define ST7735_DC_PIN  GPIO_PIN_0

#define ST7735_CS_PORT GPIOA
#define ST7735_CS_PIN  GPIO_PIN_1

#define ST7735_RST_PORT GPIOA
#define ST7735_RST_PIN  GPIO_PIN_4

#endif


#include "main.h"
#include "pin_defines.h"
#include "st7735.h"

static const uint16_t init_table[] = {
  CMD(ST7735_FRMCTR1), 0x01, 0x2c, 0x2d,
  CMD(ST7735_FRMCTR2), 0x01, 0x2c, 0x2d,
  CMD(ST7735_FRMCTR3), 0x01, 0x2c, 0x2d, 0x01, 0x2c, 0x2d,
  CMD(ST7735_INVCTR), 0x07,
  CMD(ST7735_PWCTR1), 0xa2, 0x02, 0x84,
  CMD(ST7735_PWCTR2), 0xc5,
  CMD(ST7735_PWCTR3), 0x0a, 0x00,
  CMD(ST7735_PWCTR4), 0x8a, 0x2a,
  CMD(ST7735_PWCTR5), 0x8a, 0xee,
  CMD(ST7735_VMCTR1), 0x0e,
  CMD(ST7735_GAMCTRP1), 0x0f, 0x1a, 0x0f, 0x18, 0x2f, 0x28, 0x20, 0x22,
                         0x1f, 0x1b, 0x23, 0x37, 0x00, 0x07, 0x02, 0x10,
  CMD(ST7735_GAMCTRN1), 0x0f, 0x1b, 0x0f, 0x17, 0x33, 0x2c, 0x29, 0x2e,
                         0x30, 0x30, 0x39, 0x3f, 0x00, 0x07, 0x03, 0x10,
  CMD(0xf0), 0x01,
  CMD(0xf6), 0x00,
  CMD(ST7735_COLMOD), 0x05,
  CMD(ST7735_MADCTL), 0xa0,
};

static void st7735_cmd(uint8_t cmd)
{
	ST7735_COMMAND_TYPE;
	SPI_START;
	HAL_SPI_Transmit(&st7735_spi, &cmd, 1, HAL_MAX_DELAY);
	SPI_STOP;
}

static void st7735_data8(uint8_t data)
{
	ST7735_DATA_TYPE;
	SPI_START;
	HAL_SPI_Transmit(&st7735_spi, &data, 1, HAL_MAX_DELAY);
	SPI_STOP;
}

static void st7735_data16(uint16_t data)
{
	st7735_data8(data >> 8);
	st7735_data8(data);
}

static void st7735_send(uint16_t value)
{
	if (value & 0x100)
		st7735_cmd(value);
	else
		st7735_data8(value);
}

static void st7735_set_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	st7735_cmd(ST7735_CASET);
	st7735_data16(ST7735_OFFSET_X + x);
	st7735_data16(ST7735_OFFSET_X + x + width - 1);

	st7735_cmd(ST7735_RASET);
	st7735_data16(ST7735_OFFSET_Y + y);
	st7735_data16(ST7735_OFFSET_Y + y + height - 1);
}

static void st7735_background_frame(void)
{
	ST7735_Draw_Rectangle(0, 0, ST7735_WIDTH, ST7735_HEIGHT, ST7735_BLACK);

	for (uint16_t x = 0; x < ST7735_WIDTH; x += VOLTAGE_EDGE_DISTANCE)
		ST7735_Draw_VerticalLine(x, 0, ST7735_HEIGHT, ST7735_GRAY);
	ST7735_Draw_VerticalLine(ST7735_WIDTH-1, 0, ST7735_HEIGHT, ST7735_GRAY);

	for (uint16_t y = 0; y < ST7735_HEIGHT; y += VOLTAGE_EDGE_DISTANCE)
		ST7735_Draw_HorizontalLine(0, y, ST7735_WIDTH, ST7735_GRAY);
	ST7735_Draw_HorizontalLine(0, ST7735_HEIGHT-1, ST7735_WIDTH, ST7735_GRAY);

}

void ST7735_Init(void)
{
	HAL_GPIO_WritePin(ST7735_RST_PORT, ST7735_RST_PIN, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(ST7735_RST_PORT, ST7735_RST_PIN, GPIO_PIN_SET);
	HAL_Delay(100);

	for (uint16_t i = 0; i < sizeof(init_table)/sizeof(init_table[0]); i++)
		st7735_send(init_table[i]);

	HAL_Delay(200);

	st7735_cmd(ST7735_SLPOUT);
	HAL_Delay(120);

	st7735_cmd(ST7735_DISPON);

	st7735_background_frame();
}

void ST7735_Draw_Rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
	st7735_set_window(x, y, width, height);

	st7735_cmd(ST7735_RAMWR);
	for (uint16_t i = 0; i < width*height; i++)
		st7735_data16(color);
}

void ST7735_Draw_Pixel(uint16_t x, uint16_t y, uint16_t color)
{
	ST7735_Draw_Rectangle(x, y, 1, 1, color);
}

void ST7735_Draw_VerticalLine(uint16_t x, uint16_t yo, uint16_t height, uint16_t color)
{
	for (uint16_t y = yo; y < height; y++)
		ST7735_Draw_Pixel(x, y, color);
}

void ST7735_Draw_HorizontalLine(uint16_t xo, uint16_t y, uint16_t width, uint16_t color)
{
	for (uint16_t x = xo; x < width; x++)
		ST7735_Draw_Pixel(x, y, color);
}

void ST7735_Draw_FFT(float *fft_values, size_t fft_buffer_size, uint16_t color)
{
	static uint16_t last_y[ST7735_WIDTH];

	if (fft_buffer_size > ST7735_WIDTH)
		fft_buffer_size = ST7735_WIDTH;

	float resolution_y = 5.0f;

	uint16_t y;

	for (uint16_t x = 5; x < ST7735_WIDTH; x++) {
		if (((x % VOLTAGE_EDGE_DISTANCE) == 0) || ((last_y[x-5] % VOLTAGE_EDGE_DISTANCE) == 0))
			ST7735_Draw_Pixel(x, last_y[x-5], ST7735_GRAY);
		else
			ST7735_Draw_Pixel(x, last_y[x-5], ST7735_BLACK);
	}

	for (uint16_t x = 5; x < fft_buffer_size; x++) {
		y = 112 - (fft_values[x-5] * resolution_y);
		ST7735_Draw_Pixel(x, y, color);
		last_y[x-5] = y;

		/*
		for (y = 0; y < height; y++)
			ST7735_Draw_Pixel(x+5, 112-y, color);
		last_height[x] = height;
		*/
	}
}

void ST7735_Draw_Voltage(float *voltage_values, size_t voltage_buffer_size, uint16_t color, uint16_t vol_resolution)
{
	static uint16_t last_y[ST7735_WIDTH];

	if (voltage_buffer_size > ST7735_WIDTH)
		voltage_buffer_size = ST7735_WIDTH;

	float resolution_y = (float)VOLTAGE_EDGE_DISTANCE / (vol_resolution / 1000); // vol_resolution [mV] -> rozdzielczosc y (vol_resolution na jedna kratke)
	uint16_t y;

	for (uint16_t x = 0; x < ST7735_WIDTH; x++) {
		if (((x % VOLTAGE_EDGE_DISTANCE) == 0) || ((last_y[x] % VOLTAGE_EDGE_DISTANCE) == 0))
			ST7735_Draw_Pixel(x, last_y[x], ST7735_GRAY);
		else
			ST7735_Draw_Pixel(x, last_y[x], ST7735_BLACK);
	}

	for (uint16_t x = 0; x < ST7735_WIDTH; x++) {
		y = ST7735_HEIGHT_HALF - (voltage_values[x] * resolution_y);

		ST7735_Draw_Pixel(x, y, color);
		last_y[x] = y;
	}
}

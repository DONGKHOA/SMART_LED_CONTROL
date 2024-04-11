#include "ili9341.h"
#include "stm32f1xx_hal.h"
#include <stdbool.h>



extern SPI_HandleTypeDef hspi1;

static volatile bool txComplete;

static void SetWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
static void WriteCommand(uint8_t command);
static void WriteData(uint8_t data);

static void WriteCommand(uint8_t command)
{
	HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &command, 1U, 100U);
}

static void WriteData(uint8_t data)
{
	HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi1, &data, 1U, 200U);
}

void ILI9341Reset(void)
{
	HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_RESET);
	HAL_Delay(200UL);
	HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_SET);
	HAL_Delay(200UL);
}

void ILI9341Init(void)
{
	HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_RESET);

	WriteCommand(0x01U);
	HAL_Delay(1000UL);
	WriteCommand(0xCBU);
	WriteData(0x39U);
	WriteData(0x2CU);
	WriteData(0x00U);
	WriteData(0x34U);
	WriteData(0x02U);
	WriteCommand(0xCFU);
	WriteData(0x00U);
	WriteData(0xC1U);
	WriteData(0x30U);
	WriteCommand(0xE8U);
	WriteData(0x85U);
	WriteData(0x00U);
	WriteData(0x78U);
	WriteCommand(0xEAU);
	WriteData(0x00U);
	WriteData(0x00U);
	WriteCommand(0xEDU);
	WriteData(0x64U);
	WriteData(0x03U);
	WriteData(0x12U);
	WriteData(0x81U);
	WriteCommand(0xF7U);
	WriteData(0x20U);
	WriteCommand(0xC0U);
	WriteData(0x23U);
	WriteCommand(0xC1U);
	WriteData(0x10U);
	WriteCommand(0xC5U);
	WriteData(0x3EU);
	WriteData(0x28U);
	WriteCommand(0xC7U);
	WriteData(0x86U);
	WriteCommand(0x36U);
	WriteData(0x48U);
	WriteCommand(0x3AU);
	WriteData(0x55U);
	WriteCommand(0xB1U);
	WriteData(0x00U);
	WriteData(0x18U);
	WriteCommand(0xB6U);
	WriteData(0x08U);
	WriteData(0x82U);
	WriteData(0x27U);
	WriteCommand(0xF2U);
	WriteData(0x00U);
	WriteCommand(0x26U);
	WriteData(0x01U);
	WriteCommand(0xE0U);
	WriteData(0x0FU);
	WriteData(0x31U);
	WriteData(0x2BU);
	WriteData(0x0CU);
	WriteData(0x0EU);
	WriteData(0x08U);
	WriteData(0x4EU);
	WriteData(0xF1U);
	WriteData(0x37U);
	WriteData(0x07U);
	WriteData(0x10U);
	WriteData(0x03U);
	WriteData(0x0EU);
	WriteData(0x09U);
	WriteData(0x00U);
	WriteCommand(0xE1U);
	WriteData(0x00U);
	WriteData(0x0EU);
	WriteData(0x14U);
	WriteData(0x03U);
	WriteData(0x11U);
	WriteData(0x07U);
	WriteData(0x31U);
	WriteData(0xC1U);
	WriteData(0x48U);
	WriteData(0x08U);
	WriteData(0x0FU);
	WriteData(0x0CU);
	WriteData(0x31U);
	WriteData(0x36U);
	WriteData(0x0FU);
	WriteCommand(0x11U);
	HAL_Delay(120UL);
	WriteCommand(0x29U);
	WriteCommand(0x36U);
	WriteData(0x48U);
}

static void SetWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
	WriteCommand(0x2AU);
	WriteData(xStart >> 8);
	WriteData(xStart);
	WriteData(xEnd >> 8);
	WriteData(xEnd);

	WriteCommand(0x2BU);
	WriteData(yStart >> 8);
	WriteData(yStart);
	WriteData(yEnd >> 8);
	WriteData(yEnd);

	WriteCommand(0x2CU);

	HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_SET);
}

void ILI9341Pixel(uint16_t x, uint16_t y, colour_t colour)
{
	colour_t beColour = __builtin_bswap16(colour);

	if (x >= ILI9341_LCD_WIDTH || y >= ILI9341_LCD_HEIGHT)
	{
		return;
	}

	SetWindow(x, y, x, y);

	HAL_SPI_Transmit(&hspi1, (uint8_t *)&beColour, 2U, 100UL);
}

uint8_t SPI_Master_write_color(uint16_t color, uint16_t size)
{
	static uint8_t Byte[1024];
	uint16_t index = 0;
	for(uint16_t i = 0; i < size; i++)
    {
		Byte[index++] = (color >> 8) & 0xFF;
		Byte[index++] = color & 0xFF;
	}
	HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_SET);
	return HAL_SPI_Transmit(&hspi1, Byte, size*2, 200U);
}

void ILI9341DrawColourBitmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *imageData)
{
	uint16_t bytestToWrite;

	SetWindow(x, y, x + width - 1U, y + height - 1U);
	bytestToWrite = width * height * 2U;
	WriteData(bytestToWrite);
}

void ILI9341FilledRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, colour_t colour)
{
	SetWindow(x, y, x + width - 1U, y + height - 1U);
    for(int i = x; i < x + width -1; i++)
    {
        uint16_t size = height +2;
        SPI_Master_write_color(colour, size);
    }
}
//void ILI9341DrawColourBitmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *imageData) {
//
//    SetWindow(x, y, x + width - 1U, y + height - 1U);
//
//
//    HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_SET);
//
//
//    for (uint32_t i = x; i < x + width ; i += 2)
//        uint16_t color = imageData;
//
//
//        HAL_SPI_Transmit(&hspi1, (uint8_t*)color, sizeof(color), 200U);
//    }
//}


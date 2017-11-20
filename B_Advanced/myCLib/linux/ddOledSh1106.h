#ifndef DD_OLED_SH_1106_H_
#define DD_OLED_SH_1106_H_
#include "deviceDrivers.h" 

class DdOledSh1106
{
private:
	/* Fixed config by device */
	const uint8_t SLAVE_ADDRESS = 0x3C;
	const char *I2C_DEV_NAME = "/dev/i2c-1";
	const uint8_t WIDTH  = 128;
	const uint8_t HEIGHT = 64;
	const uint8_t CONTROL_COMMAND = 0x00;
	const uint8_t CONTROL_DATA    = 0x40;
	const uint8_t PAGE_SIZE = 8;

	const uint8_t SH1106_SETCONTRAST = 0x81;
	const uint8_t SH1106_DISPLAYALLON_RESUME = 0xA4;
	const uint8_t SH1106_DISPLAYALLON = 0xA5;
	const uint8_t SH1106_NORMALDISPLAY = 0xA6;
	const uint8_t SH1106_INVERTDISPLAY = 0xA7;
	const uint8_t SH1106_DISPLAYOFF = 0xAE;
	const uint8_t SH1106_DISPLAYON = 0xAF;
	const uint8_t SH1106_SETDISPLAYOFFSET = 0xD3;
	const uint8_t SH1106_SETCOMPINS = 0xDA;
	const uint8_t SH1106_SETVCOMDETECT = 0xDB;
	const uint8_t SH1106_SETDISPLAYCLOCKDIV = 0xD5;
	const uint8_t SH1106_SETPRECHARGE = 0xD9;
	const uint8_t SH1106_SETMULTIPLEX = 0xA8;
	const uint8_t SH1106_SETLOWCOLUMN = 0x00;
	const uint8_t SH1106_SETHIGHCOLUMN = 0x10;
	const uint8_t SH1106_SETSTARTLINE = 0x40;
	const uint8_t SH1106_MEMORYMODE = 0x20;
	const uint8_t SH1106_COLUMNADDR = 0x21;
	const uint8_t SH1106_PAGEADDR   = 0x22;
	const uint8_t SH1106_COMSCANINC = 0xC0;
	const uint8_t SH1106_COMSCANDEC = 0xC8;
	const uint8_t SH1106_SEGREMAP = 0xA0;
	const uint8_t SH1106_CHARGEPUMP = 0x8D;
	const uint8_t SH1106_SWITCHCAPVCC = 0x2;
	const uint8_t SH1106_NOP = 0xE3;

	/* config for printing text */
	const uint8_t TEXT_AREA_ORIGIN_X = 0;
	const uint8_t TEXT_AREA_ORIGIN_Y = 0;
	const uint8_t TEXT_AREA_END_X = 128;
	const uint8_t TEXT_AREA_END_Y = 64;
	const uint8_t FONT_WIDTH = 5;
	const uint8_t FONT_HEIGHT = 8;

private:
	static DdOledSh1106 s_instance;
	int m_fd;

	// x, y is pixel
	uint8_t m_currentPosX;
	uint8_t m_currentPosY;

private:
	DdOledSh1106();
	~DdOledSh1106();
	RET drawChar(char c);
	RET sendCommand(uint8_t cmd);
	RET sendData(uint8_t data);

public:
	static DdOledSh1106* getInstance();
	RET initialize();
	RET setPos(uint8_t x, uint8_t y);
	RET fillRect(uint8_t DdOledSh1106, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
	RET setCharPos(uint8_t textX, uint8_t textY);
	RET putChar(char c);
	RET printText(const char* text);
};

#endif /* DD_OLED_SH_1106_H_ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include </usr/include/linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "ddOledSh1106.h"

/* memo:
 * i2cdetect -y 1
 * sudo apt-get install libi2c-dev
 */

DdOledSh1106 DdOledSh1106::s_instance;

DdOledSh1106::DdOledSh1106()
{
}

DdOledSh1106::~DdOledSh1106()
{
}

DdOledSh1106* DdOledSh1106::getInstance()
{
	return &s_instance;
}

RET DdOledSh1106::initialize()
{
	if((m_fd = open(I2C_DEV_NAME,O_RDWR)) < 0){
		printf("i2c error\n");
		return RET_ERR;
	}
	if (ioctl(m_fd, I2C_SLAVE, SLAVE_ADDRESS) < 0) {
		fprintf(stderr,"i2c error\n");
		return RET_ERR;
	}

	m_currentPosX = TEXT_AREA_ORIGIN_X;
	m_currentPosY = TEXT_AREA_ORIGIN_Y;

	/* init sequence */
	sendCommand(SH1106_DISPLAYOFF);
	sendCommand(SH1106_SETMULTIPLEX);
	sendCommand(0x3F);
	sendCommand(SH1106_SETDISPLAYOFFSET);
	sendCommand(0);
	sendCommand(SH1106_SETSTARTLINE | 0x00);
	sendCommand(SH1106_SEGREMAP | 0x01);
	sendCommand(SH1106_COMSCANDEC);
	sendCommand(SH1106_SETCOMPINS);
	sendCommand(0x12);
	sendCommand(SH1106_SETCONTRAST);
	sendCommand(0xCF);
	sendCommand(SH1106_DISPLAYALLON_RESUME);
	sendCommand(SH1106_NORMALDISPLAY);
	sendCommand(SH1106_SETDISPLAYCLOCKDIV);
	sendCommand(0x80);
	sendCommand(SH1106_CHARGEPUMP);
	sendCommand(0x14);
	//  sendCommand(SH1106_MEMORYMODE);
	//  sendCommand(0x00);  // vertical addressing mode
	sendCommand(0x02);  /* column address = 2 */
	sendCommand(0x10);
	sendCommand(0x40);  /* display start line = 0 */
	sendCommand(0xB0);  /* page start = 0 */
	sendCommand(SH1106_DISPLAYON);

	fillRect(0, 0, 0, WIDTH, HEIGHT);
	printText("Hello");
	return RET_OK;
}


// y must be 8 align
RET DdOledSh1106::setPos(uint8_t x, uint8_t y)
{
	x += 2; /* column pos offset of 2 */
	sendCommand(0x10 | (uint8_t)(x >> 4)); /* higher column address */
	sendCommand(x&0x0F);          /* lower column address */
	sendCommand(0xB0 + (y / PAGE_SIZE));  /* row address */
	return RET_OK;
}

// y must be 8 align
RET DdOledSh1106::fillRect(uint8_t on, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	uint8_t data = (on == 0 ? 0 : 0xff);

	for(uint8_t y = y0; y < y1; y += PAGE_SIZE) {
		setPos(x0, y);
		for(uint8_t x = x0; x < x1; x++) {
			sendData(data);
		}
	}
	return RET_OK;
}

RET DdOledSh1106::setCharPos(uint8_t textX, uint8_t textY)
{
	m_currentPosX = TEXT_AREA_ORIGIN_X + textX * FONT_WIDTH;
	m_currentPosY = TEXT_AREA_ORIGIN_Y + textY * FONT_HEIGHT;
	if( m_currentPosX > TEXT_AREA_END_X ) {
		m_currentPosX = TEXT_AREA_ORIGIN_X;
	}
	if( m_currentPosY > TEXT_AREA_END_Y ) {
		m_currentPosY = TEXT_AREA_ORIGIN_Y;
	}

  return RET_OK;
}

RET DdOledSh1106::putChar(char c)
{
	uint8_t endX = m_currentPosX + FONT_WIDTH;
	uint8_t endY = m_currentPosY + FONT_HEIGHT;
	if(c == '\r' || c == '\n') {
	  endX = TEXT_AREA_END_X + 1;
	}
	if( endX > TEXT_AREA_END_X ) {
		m_currentPosX = TEXT_AREA_ORIGIN_X;
		endX = m_currentPosX + FONT_WIDTH;
		m_currentPosY += FONT_HEIGHT;
		endY = m_currentPosY + FONT_HEIGHT;
		if( endY > TEXT_AREA_END_Y ) {
			m_currentPosY = TEXT_AREA_ORIGIN_Y;
			endY = m_currentPosY + FONT_HEIGHT;
		}
		fillRect(0, TEXT_AREA_ORIGIN_X, m_currentPosY, TEXT_AREA_END_X, endY);
	}

	setPos(m_currentPosX, m_currentPosY);
	if(c == '\r' || c == '\n') {
		return RET_OK;
	}
	drawChar(c);

	m_currentPosX = endX;

	return RET_OK;
}

RET DdOledSh1106::printText(const char* text)
{
	while(*text != '\0') {
		putChar(*text);
		text++;
	}
	return RET_OK;
}

RET DdOledSh1106::sendData(uint8_t data) {
	uint8_t buf[2];
	buf[0] = CONTROL_DATA;
	buf[1] = data;
	if (write(m_fd, buf, 2) != 2) {
		return RET_ERR;
	}
	return RET_OK;
}

RET DdOledSh1106::sendCommand(uint8_t cmd) {
	uint8_t buf[2];
	buf[0] = CONTROL_COMMAND;
	buf[1] = cmd;
	if (write(m_fd, buf, 2) != 2) {
		return RET_ERR;
	}
	return RET_OK;
}

RET DdOledSh1106::drawChar(char c)
{
	extern unsigned char font[];
	for (uint8_t i =0; i<FONT_WIDTH; i++ ) {	
		uint8_t line = *(font+(c*FONT_WIDTH)+i);
		sendData(line);
	}
	return RET_OK;
}


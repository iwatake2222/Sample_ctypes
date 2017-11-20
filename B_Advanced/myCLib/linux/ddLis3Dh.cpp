#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include </usr/include/linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "ddLis3Dh.h"

/* memo:
 * i2cdetect -y 1
 * sudo apt-get install libi2c-dev
 */

DdLis3Dh DdLis3Dh::s_instance;

DdLis3Dh::DdLis3Dh()
{
}


DdLis3Dh::~DdLis3Dh()
{
}

DdLis3Dh* DdLis3Dh::getInstance()
{
	return &s_instance;
}

RET DdLis3Dh::initialize()
{
	if((m_fd = open(I2C_DEV_NAME,O_RDWR)) < 0){
		printf("i2c error\n");
		return RET_ERR;
	}
	if (ioctl(m_fd, I2C_SLAVE, SLAVE_ADDRESS) < 0) {
		fprintf(stderr,"i2c error\n");
		return RET_ERR;
	}
	int whoami = i2c_smbus_read_byte_data(m_fd, 0x0f);
	printf("LIS3DH is %02X\n", whoami);

	/* Configure MEMS: 1600 Hz, XYZ = en */
	unsigned char ctrl = (unsigned char) ( (9 << 4) | (0x07) );
	i2c_smbus_write_byte_data(m_fd, 0x20, ctrl);

	return RET_OK;
}

RET DdLis3Dh::read(float *x, float *y, float *z, int *tap)
{
	uint8_t low, high;
	low = i2c_smbus_read_byte_data(m_fd, 0x28);
	high = i2c_smbus_read_byte_data(m_fd, 0x29);
	*x = (int16_t)(((high & 0xff) << 8) | (low & 0xff));
	low = i2c_smbus_read_byte_data(m_fd, 0x2A);
	high = i2c_smbus_read_byte_data(m_fd, 0x2B);
	*y = (int16_t)(((high & 0xff) << 8) | (low & 0xff));
	low = i2c_smbus_read_byte_data(m_fd, 0x2C);
	high = i2c_smbus_read_byte_data(m_fd, 0x2D);
	*z = (int16_t)(((high & 0xff) << 8) | (low & 0xff));

	*x /= 16384.0;
	*y /= 16384.0;
	*z /= 16384.0;

	// printf("%05.3f %05.3f %05.3f\n", *x/16384.0, *y/16384.0, *z/16384.0);
	return RET_OK;
}

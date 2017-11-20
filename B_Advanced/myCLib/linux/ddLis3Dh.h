#ifndef DD_LIS_3_DH_H_
#define DD_LIS_3_DH_H_
#include "deviceDrivers.h" 

class DdLis3Dh
{
private:
	const int SLAVE_ADDRESS = 0x18;
	const char *I2C_DEV_NAME = "/dev/i2c-1";
	static DdLis3Dh s_instance;
	int m_fd;

private:
	DdLis3Dh();
	~DdLis3Dh();

public:
	static DdLis3Dh* getInstance();
	RET initialize();
	RET read(float *x, float *y, float *z, int *tap);
};


#endif /* DD_LIS_3_DH_H_ */

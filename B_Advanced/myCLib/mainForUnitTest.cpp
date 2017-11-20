#include <stdio.h>
#include "deviceDrivers.h"
#include "unistd.h"

int main()
{
	RET ret = RET_OK;
	DeviceStatus deviceStatus;
	printf("Hello\n");
	printf("Version = %d\n", getVersion());
	
	ret = initialize();
	if(ret == RET_OK) {
		printf("device driver initialize OK\n");
	} else {
		printf("device driver initialize ERROR\n");
	}

	getDeviceStatus(&deviceStatus);
	printf("led 0 = %d\n", deviceStatus.led_0);
	setLed(0, true);
	getDeviceStatus(&deviceStatus);
	printf("led 0 = %d\n", deviceStatus.led_0);

	getchar();
	
	ret = finalize();
	if(ret == RET_OK) {
		printf("device driver finalize OK\n");
	} else {
		printf("device driver finalize ERROR\n");
	}
	
	getchar();
	return 0;
}

#ifndef DEVICE_DRIVER_H_
#define DEVICE_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_LED_0 5
#define GPIO_LED_1 6
#define GPIO_COLOR_LED_R 26
#define GPIO_COLOR_LED_G 19
#define GPIO_COLOR_LED_B 13
#define GPIO_SPEAKER 16
#define GPIO_SERVO 18
#define GPIO_MOTOR 12
#define GPIO_BTN_0 20
#define GPIO_BTN_1 21

typedef struct {
	/* output device */
	bool led_0;
	bool led_1;
	int  led_r;
	int  led_g;
	int  led_b;
	int  speaker;
	int  motor;
	int  servo;
	char* oledText;

	/* input device */
	bool btn_0;
	bool btn_1;
	float accel_x;
	float accel_y;
	float accel_z;
	int tap;
} DeviceStatus;

typedef signed int     int32_t;
typedef unsigned int   uint32_t;
typedef signed short   int16_t;
typedef unsigned short uint16_t;
typedef signed char    int8_t;
typedef unsigned char  uint8_t;

typedef int RET;
#define RET_OK  0x00000000
#define RET_ERR 0x80000001


int getVersion();
RET initialize();
RET finalize();
RET setLed(int num, bool onoff);
RET setColorLED(int r, int g, int b);
RET setSpeaker(int f);
RET setMotor(int motor);
RET setServo(int servo);
RET setOledText(const char* text);
RET getDeviceStatus(DeviceStatus* deviceStatus);

#ifdef __cplusplus
}
#endif

#endif /* DEVICE_DRIVER_H_ */


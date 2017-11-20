#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "deviceDrivers.h" 
#include "ddLis3Dh.h"
#include "ddOledSh1106.h"

/*** constant values ***/
const int SERVO_PWM_CLOCK = 400;
const int SERVO_PWM_RANGE = 100;
const int SERVO_PWM_MIN = 24;
const int SERVO_PWM_MAX = 115;

/*** internal variables ***/
static bool g_isExit = false;
static pthread_t g_threadDeviceStatusUpdate;
static pthread_t g_threadSpeaker;
static DeviceStatus g_deviceStatus = {0};

/*** internal functions ***/
void *threadDeviceStatusUpdateFunc(void *arg);
void *threadSpeakerFunc(void *arg);
static RET initializePort();
static RET initializeDevice();
static RET updateDeviceStatus();


RET setLed(int num, bool onoff)
{
	int port = GPIO_LED_0;
	switch (num) {
	case 0:
	default:
		g_deviceStatus.led_0 = onoff;
		port = GPIO_LED_0;
		break;
	case 1:
		g_deviceStatus.led_1 = onoff;
		port = GPIO_LED_1;
	}
	printf("out port[%d] is %d\n", port, onoff);
	digitalWrite(port, onoff);
	return RET_OK;
}

/* rgb is 0 - 100 */
RET setColorLED(int r, int g, int b)
{
	g_deviceStatus.led_r = r;
	g_deviceStatus.led_g = g;
	g_deviceStatus.led_b = b;
	printf("prm rgb = %d %d %d\n", r, g, b);
	softPwmWrite(GPIO_COLOR_LED_R, r);
	softPwmWrite(GPIO_COLOR_LED_G, g);
	softPwmWrite(GPIO_COLOR_LED_B, b);
	return RET_OK;
}

/* f is frequency */
RET setSpeaker(int f)
{
	g_deviceStatus.speaker = f;
	printf("speaker = %d[Hz]\n", f);
	return RET_OK;
}

/* motor is 0 - 100 */
RET setMotor(int motor)
{
	g_deviceStatus.motor = motor;
	printf("motor duty is %d[%%]\n", motor);
	softPwmWrite(GPIO_MOTOR, motor);
	return RET_OK;
}

/* servo i 0 - 100 */
RET setServo(int servo)
{
	g_deviceStatus.servo = servo;
	printf("servo position is %d\n", servo);
	int delta = (servo * (SERVO_PWM_MAX - SERVO_PWM_MIN)) / 100;
	delta += SERVO_PWM_MIN;
	// softPwmWrite(GPIO_SERVO, servo);
	return RET_OK;
}

RET setOledText(const char* text)
{
	mempcpy(g_deviceStatus.oledText, text, 64);
	g_deviceStatus.oledText[63] = '\0';	// just in case
	DdOledSh1106::getInstance()->setCharPos(0, 0);
	DdOledSh1106::getInstance()->printText(text);
	DdOledSh1106::getInstance()->printText("                ");
	
	return RET_OK;
}

RET getDeviceStatus(DeviceStatus* deviceStatus)
{
	deviceStatus->led_0    = g_deviceStatus.led_0;
	deviceStatus->led_1    = g_deviceStatus.led_1;
	deviceStatus->led_r    = g_deviceStatus.led_r;
	deviceStatus->led_g    = g_deviceStatus.led_g;
	deviceStatus->led_b    = g_deviceStatus.led_b;
	deviceStatus->speaker  = g_deviceStatus.speaker;
	deviceStatus->motor    = g_deviceStatus.motor;
	deviceStatus->servo    = g_deviceStatus.servo;
	deviceStatus->oledText = g_deviceStatus.oledText;
	deviceStatus->btn_0    = g_deviceStatus.btn_0;
	deviceStatus->btn_1    = g_deviceStatus.btn_1;
	deviceStatus->accel_x  = g_deviceStatus.accel_x;
	deviceStatus->accel_y  = g_deviceStatus.accel_y;
	deviceStatus->accel_z  = g_deviceStatus.accel_z;
	deviceStatus->tap      = g_deviceStatus.tap;
	return RET_OK;
}

RET updateDeviceStatus()
{
	g_deviceStatus.btn_0 = digitalRead(GPIO_BTN_0);
	g_deviceStatus.btn_1 = digitalRead(GPIO_BTN_1);
	DdLis3Dh::getInstance()->read(&g_deviceStatus.accel_x, &g_deviceStatus.accel_y, &g_deviceStatus.accel_z, &g_deviceStatus.tap);
	return RET_OK;
}

RET initializePort()
{
	if( wiringPiSetupGpio() == -1 ) return RET_ERR;
	pinMode(GPIO_LED_0, OUTPUT);
	pinMode(GPIO_LED_1, OUTPUT);
	softPwmCreate (GPIO_COLOR_LED_R, 0, 100);
	softPwmCreate (GPIO_COLOR_LED_G, 0, 100);
	softPwmCreate (GPIO_COLOR_LED_B, 0, 100);
	pinMode(GPIO_SPEAKER, OUTPUT);
	// softPwmCreate (GPIO_SERVO, 0, SERVO_PWM_RANGE) ;
	// pinMode(GPIO_SERVO, OUTPUT);
	// pinMode(GPIO_SERVO, PWM_OUTPUT);
	// pwmSetMode(PWM_MODE_MS);
	// pwmSetClock(SERVO_PWM_CLOCK);
	// pwmSetRange(SERVO_PWM_RANGE);
	softPwmCreate (GPIO_MOTOR, 0, 100);

	pinMode(GPIO_BTN_0, INPUT);
	pinMode(GPIO_BTN_1, INPUT);
	pullUpDnControl(GPIO_BTN_0, PUD_UP);
	pullUpDnControl(GPIO_BTN_1, PUD_UP);
	return RET_OK;
}

RET initializeDevice()
{
	RET ret = RET_OK;
	ret |= DdLis3Dh::getInstance()->initialize();
	ret |= DdOledSh1106::getInstance()->initialize();
	return ret;
}

RET initialize()
{
	RET ret = RET_OK;
	printf("C Lib initialize Start\n");
	ret |= initializePort();
	ret |= initializeDevice();
	g_deviceStatus.oledText = new char[64];
	g_deviceStatus.oledText[0] = '\0';

	pthread_attr_t tattr;
	pthread_attr_init(&tattr);
	struct sched_param param;
	param.sched_priority = 0;
	pthread_attr_setschedpolicy(&tattr, SCHED_RR);
	pthread_attr_setschedparam(&tattr, &param);
	pthread_create(&g_threadDeviceStatusUpdate, &tattr, &threadDeviceStatusUpdateFunc, NULL);
	pthread_create(&g_threadSpeaker, &tattr, &threadSpeakerFunc, NULL);

	printf("C Lib initialize Done\n");
	return RET_OK;
}

RET finalize()
{
	printf("C Lib finalize Start\n");
	if(g_isExit == false) {
		g_isExit = true;
		pthread_join(g_threadDeviceStatusUpdate, NULL);
		pthread_join(g_threadSpeaker, NULL);
	}
	if(g_deviceStatus.oledText != NULL) delete g_deviceStatus.oledText;
	g_deviceStatus.oledText = NULL;
	printf("C Lib finalize Done\n");
	return RET_OK;
}

void *threadDeviceStatusUpdateFunc(void *arg)
{
	while(!g_isExit) {
		usleep(100 * 1000);
		updateDeviceStatus();
	}
	return 0;
}

void *threadSpeakerFunc(void *arg)
{
	bool level = false;
	while(!g_isExit) {
		if(g_deviceStatus.speaker > 20) {
			usleep(1000000/g_deviceStatus.speaker);
			if(level == false) {
				digitalWrite(GPIO_SPEAKER, 1);
			} else {
				digitalWrite(GPIO_SPEAKER, 0);
			}
			level = !level;
		}
	}
	return 0;
}

int getVersion(){
	printf("Device Driver on Windows\n");
	return 1;
}

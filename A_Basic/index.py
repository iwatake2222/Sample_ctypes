#!/bin/env python
# coding: utf-8
import json
from bottle import route, run, request, HTTPResponse, template, static_file
import RPi.GPIO
import atexit

GPIO_PORT_LED_0 = 5
GPIO_PORT_LED_1 = 6
GPIO_PORT_BTN_0 = 20
GPIO_PORT_BTN_1 = 21

@route('/static/:path#.+#', name='static')
def static(path):
	return static_file(path, root='static')

@route('/')
def root():
	return template("index")

# curl -H "Accept: application/json" -H "Content-type: application/json" -X POST -d '{"num":"0", "onoff":true}' http://192.168.1.88:8080/setLed
@route('/setLed', method='POST')
def setLedEntry():
	var = request.json
	# print (var)
	if (var["num"] == "0" ):
		RPi.GPIO.output(GPIO_PORT_LED_0, var["onoff"])
	elif (var["num"] == "1" ):
		RPi.GPIO.output(GPIO_PORT_LED_1, var["onoff"])
	retBody = {"ret": "ok"}
	r = HTTPResponse(status=200, body=retBody)
	r.set_header('Content-Type', 'application/json')
	return r

# curl -H "Accept: application/json" -H "Content-type: application/json" -X POST -d '{"dummy":"0"}' http://192.168.1.88:8080/getButton
@route('/getButton', method='POST')
def getButtonEntry():
	retBody = {
		"ret": "ok",
		"btn_0": RPi.GPIO.input(GPIO_PORT_BTN_0),
		"btn_1": RPi.GPIO.input(GPIO_PORT_BTN_1)
	}
	r = HTTPResponse(status=200, body=retBody)
	r.set_header('Content-Type', 'application/json')
	return r

def main():
	print("Initialize port")
	RPi.GPIO.setmode(RPi.GPIO.BCM)
	RPi.GPIO.setup(GPIO_PORT_LED_0, RPi.GPIO.OUT) 
	RPi.GPIO.setup(GPIO_PORT_LED_1, RPi.GPIO.OUT) 
	RPi.GPIO.setup(GPIO_PORT_BTN_0, RPi.GPIO.IN, pull_up_down = RPi.GPIO.PUD_UP) 
	RPi.GPIO.setup(GPIO_PORT_BTN_1, RPi.GPIO.IN, pull_up_down = RPi.GPIO.PUD_UP) 
	RPi.GPIO.output(GPIO_PORT_LED_0, 0)
	RPi.GPIO.output(GPIO_PORT_LED_1, 0)

	print('Server Start')
	run(host='0.0.0.0', port=8080, debug=True, reloader=True)
	# run(host='0.0.0.0', port=8080, debug=False, reloader=False)

def atExit():
	print("atExit")
	RPi.GPIO.cleanup()

if __name__ == '__main__':
	atexit.register(atExit)
	main()

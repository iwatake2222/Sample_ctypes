#!/bin/env python
# coding: utf-8
import os
import sys
import platform
import time
import atexit
from ctypes import *
import json
from bottle import route, run, request, HTTPResponse, template, static_file
# import cameraSequentialCapture
import picamera
import shutil

"""
index.py
"""
__author__      = "take-iwiw"
__copyright__   = "Copyright 2017, take-iwiw"
__date__        = "16 Nov 2017"

class DeviceStatus(Structure):
	_fields_ = [
		("led_0", c_bool),
		("led_1", c_bool),
		("led_r", c_int),
		("led_g", c_int),
		("led_b", c_int),
		("speaker", c_int),
		("motor", c_int),
		("servo", c_int),
		("oled_text", c_char_p),
		("btn_0", c_bool),
		("btn_1", c_bool),
		("accel_x", c_float),
		("accel_y", c_float),
		("accel_z", c_float),
		("tap", c_int)]
	def __init__(self):
		self.led_0 = c_bool(False)
		self.led_1 = c_bool(False)
		self.led_r = c_int(0)
		self.led_g = c_int(0)
		self.led_b = c_int(0)
		self.speaker = c_int(0)
		self.motor = c_int(0)
		self.servo = c_int(0)
		self.oled_text = c_char_p(b"AAAAAAAAAA")
		self.btn_0 = c_bool(False)
		self.btn_1 = c_bool(False)
		self.accel_x = c_float(0)
		self.accel_y = c_float(0)
		self.accel_z = c_float(0)
		self.tap = c_int(0)

g_deviceStatus = DeviceStatus()
g_lib = None
camera = None

def loadCLib():
	global g_lib
	if platform.system() == "Windows":
		print("Load DLL")
		g_lib = windll.LoadLibrary("myCLib/MyDeviceDrivers.dll")
	else:
		print("Load SO")
		g_lib = cdll.LoadLibrary("myCLib/libMyDeviceDrivers.so")
	print("C Device Driver Library Version = " + str(g_lib.getVersion()))	

def updateDeviceStatus():
	global g_deviceStatus
	g_lib.getDeviceStatus(byref(g_deviceStatus))

def printDeviceStatus():
	global g_deviceStatus
	updateDeviceStatus()
	print("----------------------")
	print("led: %d %d" % (g_deviceStatus.led_0, g_deviceStatus.led_1))
	print("rgb: %d %d %d" % (g_deviceStatus.led_r, g_deviceStatus.led_g, g_deviceStatus.led_b))
	print("speaker: %d" % g_deviceStatus.speaker)
	print("motor: %d" % g_deviceStatus.motor)
	print("servo: %d" % g_deviceStatus.servo)
	print("oled: %s" % g_deviceStatus.oled_text.decode('ascii'))
	print("btn: %d %d" % (g_deviceStatus.btn_0, g_deviceStatus.btn_1))
	print("accel xyz: %f %f %f" % (g_deviceStatus.accel_x, g_deviceStatus.accel_y, g_deviceStatus.accel_z))
	print("tap: %d" % g_deviceStatus.tap)
	print("----------------------")

@route('/static/:path#.+#', name='static')
def static(path):
	return static_file(path, root='static')

@route('/')
def root():
	return template("index")
	# return 'Hello'

@route('/led', method='POST')
def ledEntry():
	var = request.json
	# print (var)
	num = int(var["num"])
	if var["onoff"] == False:
		onoff = False
	else:
		onoff = True
	g_lib.setLed(num, onoff)

@route('/setDeviceStatus', method='POST')
def setDeviceStatusEntry():
	var = request.json
	g_lib.setLed(0, bool(var["led_0"]))
	g_lib.setLed(1, bool(var["led_1"]))
	g_lib.setColorLED(int(var["color_r"]), int(var["color_g"]), int(var["color_b"]))
	g_lib.setSpeaker(int(var["speaker"]))
	g_lib.setMotor(int(var["motor"]))
	# g_lib.setServo(int(var["servo"]))
	g_lib.setOledText(c_char_p(var["text"].encode()))

@route('/getDeviceStatus', method='POST')
def getDeviceStatusEntry():
	global camera
	camera.capture("static/temp.jpg")
	shutil.copyfile("static/temp.jpg", "static/00.jpg")
	global g_deviceStatus
	updateDeviceStatus()
	# printDeviceStatus()
	var = request.json
	retBody = {
		"ret": "ok",
		"led_0": g_deviceStatus.led_0,
		"led_1": g_deviceStatus.led_1,
		"led_r": g_deviceStatus.led_r,
		"led_g": g_deviceStatus.led_g,
		"led_b": g_deviceStatus.led_b,
		"speaker": g_deviceStatus.speaker,
		"motor": g_deviceStatus.motor,
		"servo": g_deviceStatus.servo,
		"oled_text": g_deviceStatus.oled_text.decode(),
		"btn_0": g_deviceStatus.btn_0,
		"btn_1": g_deviceStatus.btn_1,
		"accel_x": g_deviceStatus.accel_x,
		"accel_y": g_deviceStatus.accel_y,
		"accel_z": g_deviceStatus.accel_z,
		"tap": g_deviceStatus.tap
	}
	r = HTTPResponse(status=200, body=retBody)
	r.set_header('Content-Type', 'application/json')
	return r

def main():
	"""
	Entry function
	:called when: the program starts
	:param none: no parameter
	:return: none
	:rtype: none
	"""
	print("Hello")
	loadCLib()
	print("init")
	ret = g_lib.initialize()
	print("g_lib.initialize(): %d" % ret)
	printDeviceStatus()

	global camera
	camera = picamera.PiCamera()
	camera.resolution = (320, 240)
	# print('Start Camera Capture Thread')
	# cameraSequentialCapture.threadStart(0.5, "static/00.jpg")

	print('Server Start')
	# run(host='0.0.0.0', port=8080, debug=True, reloader=True)
	run(host='0.0.0.0', port=8080, debug=False, reloader=False)


def atExit():
	print("atExit")
	# cameraSequentialCapture.threadStop()
	g_lib.finalize()


if __name__ == '__main__':
	atexit.register(atExit)
	main()
	# cameraSequentialCapture.threadStop()
	g_lib.finalize()
	print("exit")

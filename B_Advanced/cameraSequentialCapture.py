#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import sys
import time
import threading
import shutil
import picamera

"""
cameraSequentialCapture.py
"""
__author__      = "take-iwiw"
__copyright__   = "Copyright 2017, take-iwiw"
__date__        = "16 Nov 2017"

isThreadExit = False
threadInstance = None

def threadFunction(interval, filename):
	global isThreadExit
	camera = picamera.PiCamera()
	camera.resolution = (320, 240)
	while(isThreadExit == False):
		camera.capture("temp.jpg")
		shutil.copyfile("temp.jpg", filename)
		time.sleep(interval)

def threadStop():
	print("thread stop: start")
	global isThreadExit
	global threadInstance
	if (threadInstance != None):
		isThreadExit = True
		threadInstance.join()
		threadInstance = None
	print("thread stop: done")

def threadStart(interval, filename):
	global isThreadExit
	global threadInstance
	isThreadExit = False
	threadInstance = threading.Thread(target=threadFunction, args=(interval, filename))
	threadInstance.start()

# threadStart(0.1, "00.jpg")
# time.sleep(4)
# threadStop()

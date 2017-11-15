#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import platform
from ctypes import *

"""
main.py
"""
__author__      = "take-iwiw"
__copyright__   = "Copyright 2017, take-iwiw"
__date__        = "16 Nov 2017"

gLib = None

def loadCLib():
	if platform.system() == "Windows":
		print("Load DLL")
		gLib = windll.LoadLibrary("myCLib/myCLib.dll")
	else:
		print("Load SO")
		gLib = cdll.LoadLibrary("myCLib/libMyCLib.so")
	print("C Device Driver Library Version = " + str(gLib.getVersion()))	

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

if __name__ == '__main__':
	main()

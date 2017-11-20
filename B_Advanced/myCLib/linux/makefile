# memo:
# >LD_LIBRARY_PATH=. ./a.exe
# >LD_LIBRARY_PATH=. gdb ./a.exe

include ../makefile_config

TARGET = ../libMyDeviceDrivers.so
SRCS = deviceDrivers.cpp ddLis3Dh.cpp ddOledSh1106.cpp font.cpp
INCLUDES = -I../
SUBS =
SUBDIRS =

all: sub_target $(TARGET)

$(TARGET): $(OBJS) $(SUBS)
	$(CC) $(LDFLAGS) -shared -fPIC -lwiringPi -o $@ $(OBJS) $(SUBS) 

.PHONY: sub_target
sub_target:
ifdef SUBDIRS
	$(foreach subdir,$(SUBDIRS),cd $(subdir) && $(MAKE) && cd ../ &&) cd ./
endif

.PHONY: clean
clean:
	$(DEL) *.o *.a *.exe *.dll
ifdef SUBDIRS
	$(foreach subdir,$(SUBDIRS),cd $(subdir) && $(MAKE) clean && cd ../ &&) cd ./
endif


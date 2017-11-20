include makefile_config

TARGET = a.exe
SRCS = mainForUnitTest.cpp
INCLUDES = -I$(SUBDIR_NAME)
SUBS = 
SUBDIRS = $(SUBDIR_NAME)
LIBS += -L./ -lMyDeviceDrivers

all: sub_target $(TARGET)

$(TARGET): $(OBJS) $(SUBS)
	$(CC) $(LDFLAGS) -pthread -o $@ $(OBJS) $(SUBS) $(LIBS)

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


TOPDIR=..
GCC_BIN=$(TOPDIR)/cross_compiler/bin/

AS      = $(GCC_BIN)arm-none-eabi-as
AR      = $(GCC_BIN)arm-none-eabi-ar
CC      = $(GCC_BIN)arm-none-eabi-gcc
CPP     = $(GCC_BIN)arm-none-eabi-g++
LD      = $(GCC_BIN)arm-none-eabi-gcc
OBJCOPY = $(GCC_BIN)arm-none-eabi-objcopy

INCLUDE	= -I$(TOPDIR)/include -I$(TOPDIR)/device/ext_devices -I$(TOPDIR)/apps -I../$(TOPDIR)/mqtt-client
DEFS	= -DBSDURG -DVERSION=\""`cat $(VERSIONFILE)`"\"

LIBS+=libapp.a

all: xinu

xinu: $(LIBS)

CFLAGS  = -mcpu=cortex-a8 -mno-unaligned-access -marm -fno-stack-protector -nostdlib -c -Wall 
#CC_FLAGS = -c -g -fno-common -fmessage-length=0 -Wall -fno-exceptions -ffunction-sections -fdata-sections
#CC_FLAGS = -c -g -fno-common -fmessage-length=0 -Wall -fno-exceptions -ffunction-sections -fdata-sections

CPU = -mfpu=vfpv3-d16 -mfloat-abi=softfp
CC_FLAGS = $(CPU) -c -g -fno-common -fmessage-length=0 -Wall -fno-exceptions -fno-rtti
CC_FLAGS += -MMD -MP

CPP_FILES += thermometer_test.cpp \
	     digital_thermometer_test.cpp \
	     led_test.cpp \
	     ledservice.cpp \
	     thermometerservice.cpp \
	     services.cpp	

O_CPP_FILES=$(patsubst %.cpp, %.o, $(CPP_FILES))

.cpp.o:
	$(CPP) $(CFLAGS) $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu++98 $(INCLUDE) -o $@ $<

libapp.a: $(O_CPP_FILES) Makefile
	$(AR) cr $@ *.o
	@rm -vf *.d
	@rm -vf *.o

.PHONY: BSP clean

clean:
	@echo "============cleaning libapp==============="
	rm -vf $(LIBS)
	rm -vf *.o

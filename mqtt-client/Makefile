TOP     = ..
TARGET  = mqttcli
ifeq ($(OS), Linux)
LIBS    = -lm -lpthread
CC      = gcc
else
GCC_BIN=$(TOP)/xinu/cross_compiler/bin/

AS      = $(GCC_BIN)arm-none-eabi-as
AR      = $(GCC_BIN)arm-none-eabi-ar
CC      = $(GCC_BIN)arm-none-eabi-gcc
CPP     = $(GCC_BIN)arm-none-eabi-g++
LD      = $(GCC_BIN)arm-none-eabi-gcc
OBJCOPY = $(GCC_BIN)arm-none-eabi-objcopy
LIBS+=libmqttcli.a
endif


INCLUDE = -I$(TOP)/inc
ifneq ($(OS), Linux)
INCLUDE += -I$(TOP)/xinu/include
endif
SRC     = $(TOP)/mqtt-client/mqttclient.c \
          $(TOP)/mqtt-client/mqttrxagent.c \
          $(TOP)/mqtt-client/mqtt_os_helper.c \
	  $(TOP)/mqtt-client/mqttutil.c

ifeq ($(OS), Linux)
CFLAGS  = -g -Wall $(INCLUDE)
else
CFLAGS  = -mcpu=cortex-a8 -mno-unaligned-access -marm -fno-stack-protector -nostdlib -c -Wall $(INCLUDE)
endif

.PHONY: xinu default all clean

ifeq ($(OS), Linux)
all: default
default: $(TARGET)
else
all: xinu
xinu: $(LIBS)
endif

OBJECTS = $(patsubst %.c, %.o, $(SRC))
HEADERS = 

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

ifeq ($(OS), Linux)
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@
	@rm -vf *.o
else
libmqttcli.a: $(OBJECTS) Makefile
	$(AR) cr $@ *.o
	@rm -vf *.d
	@rm -vf *.o
endif

clean:
	@rm -vf $(TOP)/mqtt-client/$(TARGET)
	@rm -vf $(TOP)/mqtt-client/$(LIBS)
	@rm -vf $(TOP)/mqtt-client/*.o

TOP     = ..
TARGET  = edge
LIBS    = -lm -lpthread
CC      = gcc
INCLUDE = -I$(TOP)/inc
SRC     = $(PWD)/edge.c \
	  $(PWD)/events.c \
	  $(PWD)/mqtt_os_helper.c
CFLAGS  = -g -Wall $(INCLUDE)

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(SRC))
HEADERS = 

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@
	@rm -vf *.o

clean:
	-rm -f $(PWD)/*.o
	-rm -f $(TARGET)

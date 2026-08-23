CC ?= gcc
CFLAGS ?= -std=c17 -Wall -Wextra -g
CPPFLAGS ?=

TARGET := build/main.exe
BUILD_DIR := build

SOURCES := \
	App/main.c \
	BSW/CDD/CallBack_Drv.c \
	BSW/IoHwAb/Brake/Brake_Drv.c \
	BSW/IoHwAb/Dht11/Dht11_Drv.c \
	BSW/IoHwAb/Pedal/Pedal_Drv.c \
	BSW/IoHwAb/Servo/Servo_Drv.c \
	BSW/IoHwAb/Timer/Timer_Drv.c \
	BSW/Services/Callback_Mgr.c
OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCES))

INCLUDE_DIRS := \
	BSW/CDD \
	BSW/IoHwAb/Brake \
	BSW/IoHwAb/Dht11 \
	BSW/IoHwAb/Pedal \
	BSW/IoHwAb/Servo \
	BSW/IoHwAb/Timer \
	BSW/Services
CPPFLAGS += $(addprefix -I,$(INCLUDE_DIRS))

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MP -c $< -o $@

-include $(OBJECTS:.o=.d)

run:
	build/main

clean:
	rm -rf $(BUILD_DIR)

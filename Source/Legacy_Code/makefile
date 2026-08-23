# -----------------------
# Toolchain
# -----------------------
CC = arm-none-eabi-gcc	 # Trình biên dịch C cho ARM Cortex
AS = arm-none-eabi-as	 # Trình biên dịch assembly (nếu dùng riêng)

# -----------------------
# Flags
# -----------------------

MCAL_INC = $(patsubst %,-I%,$(wildcard Mcal/*))

CFLAGS = --specs=nano.specs\
         -mcpu=cortex-m3 -mthumb -std=c11 -Wall -g -O0 \
         -Iinclude \
         -IApp \
         -Ilib/CMSIS \
         -Ilib/SPL/inc \
         -IMcal \
         $(MCAL_INC) \
         -IBsw/IoHwAb \
         -IBsw/IoHwAb/Cfg \
         -IBsw/IoHwAb/lcd \
         -IBsw/IoHwAb/rc522 \
         -IBsw/IoHwAb/servo \
         -IBsw/IoHwAb/Systick \
         -IBsw/IoHwAb/usbttl \
         -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER

# -----------------------
# Sources
# -----------------------
SRC = $(wildcard lib/SPL/src/*.c) \
      $(wildcard Mcal/*.c) \
      $(wildcard Mcal/*/*.c) \
      $(wildcard Bsw/IoHwAb/Cfg/*.c) \
      $(wildcard App/*.c) \
      $(wildcard Bsw/IoHwAb/lcd/*.c) \
      $(wildcard Bsw/IoHwAb/rc522/*.c) \
      $(wildcard Bsw/IoHwAb/servo/*.c) \
      $(wildcard Bsw/IoHwAb/Systick/*.c) \
      $(wildcard Bsw/IoHwAb/usbttl/*.c)

# startup assembly
STARTUP = startup/startup_stm32f103c8tx.s

# directory for object files
OBJDIR = Object

# object filenames placed in OBJDIR
OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(SRC))) \
      $(OBJDIR)/$(notdir $(STARTUP:.s=.o))
OUT = build/main.elf	# File đầu ra

# Linker flags
LDFLAGS = -T linker.ld -Wl,-Map=build/main.map -nostartfiles

# -----------------------
# Rules
# -----------------------

$(OBJDIR)/%.o: App/%.c
    @mkdir -p $(OBJDIR)
    $(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: lib/SPL/src/%.c
    @mkdir -p $(OBJDIR)
    $(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Mcal/%.c
    @mkdir -p $(OBJDIR)
    $(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Mcal/%/%.c
    @mkdir -p $(OBJDIR)
    $(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Bsw/IoHwAb/Cfg/%.c
    @mkdir -p $(OBJDIR)
    $(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Bsw/IoHwAb/lcd/%.c
    @mkdir -p $(OBJDIR)
    $(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Bsw/IoHwAb/rc522/%.c
    @mkdir -p $(OBJDIR)
    $(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Bsw/IoHwAb/servo/%.c
    @mkdir -p $(OBJDIR)
    $(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Bsw/IoHwAb/Systick/%.c
    @mkdir -p $(OBJDIR)
    $(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Bsw/IoHwAb/usbttl/%.c
    @mkdir -p $(OBJDIR)
    $(CC) $(CFLAGS) -c $< -o $@

# assemble startup files into OBJDIR as well
$(OBJDIR)/%.o: startup/%.s
    @mkdir -p $(OBJDIR)
    $(CC) $(CFLAGS) -c $< -o $@

build: $(OBJ)
    echo "[CC] Linking all object files into $(OUT) successfully!"
    mkdir -p build
    $(CC) $(CFLAGS) $(OBJ) -o $(OUT) $(LDFLAGS) $(LIBS)

clean:
    rm -rf $(OBJDIR)/*.o startup/*.o lib/SPL/src/*.o src/*.o

flash: $(OUT)
    openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
        -c "program $(OUT) verify reset exit"
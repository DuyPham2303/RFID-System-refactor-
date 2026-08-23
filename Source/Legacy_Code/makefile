# -----------------------
# Toolchain
# -----------------------
CC = arm-none-eabi-gcc	 # Trình biên dịch C cho ARM Cortex
AS = arm-none-eabi-as	 # Trình biên dịch assembly (nếu dùng riêng)

# -----------------------
# Flags
# -----------------------

CFLAGS = --specs=nano.specs\
		 -mcpu=cortex-m3 -mthumb -std=c11 -Wall -g -O0 \
         -Iinclude \
		 -IApp \
		 -Ilib/CMSIS \
		 -Ilib/SPL/inc \
		 -IMcal \
		 -IBsw/Cfg \
		 -IBsw/lcd \
		 -IBsw/rc522 \
		 -IBsw/servo \
		 -IBsw/Systick \
		 -IBsw/usbttl \
         -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER

# -----------------------
# Sources
# -----------------------
# C source files under src/ plus SPL library
SRC = $(wildcard lib/SPL/src/*.c) \
	  $(wildcard Mcal/*.c) \
	  $(wildcard Bsw/Cfg/*.c) \
	  $(wildcard App/*.c) \
	  $(wildcard Bsw/lcd/*.c) \
	  $(wildcard Bsw/rc522/*.c) \
	  $(wildcard Bsw/servo/*.c) \
	  $(wildcard Bsw/Systick/*.c) \
	  $(wildcard Bsw/usbttl/*.c) \	

# startup assembly
STARTUP = startup/startup_stm32f103c8tx.s

# directory for object files
OBJDIR = Object

# object filenames placed in OBJDIR (keep just basename to avoid nested paths)
OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(SRC))) \
      $(OBJDIR)/$(notdir $(STARTUP:.s=.o))
OUT = build/main.elf	# File đầu ra

# Linker flags
LDFLAGS = -T linker.ld -Wl,-Map=build/main.map -nostartfiles

# -----------------------
# Rules
# -----------------------

# generic rule: compile any .c file into .o file in OBJDIR
$(OBJDIR)/%.o: App/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: lib/SPL/src/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Mcal/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Bsw/Cfg/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Bsw/lcd/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Bsw/rc522/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Bsw/servo/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Bsw/Systick/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@
	
$(OBJDIR)/%.o: Bsw/usbttl/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@
	
# assemble startup files into OBJDIR as well
$(OBJDIR)/%.o: startup/%.s
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

build: $(OBJ) # Biên dịch tất cả .o đã tạo → tạo ra file test.elf trong thư mục build/.
	echo "[CC] Linking all object files into $(OUT) successfully!"
	mkdir -p build
	$(CC) $(CFLAGS) $(OBJ) -o $(OUT) $(LDFLAGS) $(LIBS)


clean: # Xoá file .o tạm và thư mục build/.
	rm -rf $(OBJDIR)/*.o startup/*.o lib/SPL/src/*.o src/*.o 

# Dùng OpenOCD để nạp file .elf vào chip STM32 qua ST-Link.
# Kiểm tra verify, reset, rồi thoát.
flash: $(OUT)
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
		-c "program $(OUT) verify reset exit"



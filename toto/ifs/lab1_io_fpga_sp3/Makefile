##########################################################################
#
# Basic, target/architecture independent makefile for building an
# application that runs on its own stack. 
#
# YSY : correction de compilation  "Unknown CPU type"
# AMX : Portage sur pour la nouvelle version de u-boot 2016
#output name for files .bin et elf
OUTPUTNAME = reptar_io_fpga_sp3

TOOL_PREFIX	:= arm-linux-gnueabihf

NM		= $(TOOL_PREFIX)-nm
AR		= $(TOOL_PREFIX)-ar
LD		= $(TOOL_PREFIX)-ld.bfd
AS		= $(TOOL_PREFIX)-as
CC		= $(TOOL_PREFIX)-gcc
STRIP	= $(TOOL_PREFIX)-strip
OBJCOPY	= $(TOOL_PREFIX)-objcopy
OBJDUMP	= $(TOOL_PREFIX)-objdump
LDR		= $(TOOL_PREFIX)-ldr

########
# Makefile for u-boot standalone
#########

LOAD_ADDR = 0x81600000

#########################################################################

CONFIG_SHELL	:= $(shell if [ -x "$$BASH" ]; then echo $$BASH; \
		    else if [ -x /bin/bash ]; then echo /bin/bash; \
		    else echo sh; fi ; fi)

ARFLAGS = crv
DBGFLAGS= -g # -DDEBUG
OPTFLAGS= -O0 #-fomit-frame-pointer
ONEFLAGS= -fno-common -msoft-float -mword-relocations -fno-toplevel-reorder \
	  -ffixed-r9 -mno-unaligned-access -mthumb\
	  -ffunction-sections -fdata-sections 
	  

TWOFLAGS= -DCONFIG_ARM -D__ARM__ -mabi=aapcs-linux -mno-thumb-interwork -march=armv5

OBJCFLAGS += --gap-fill=0xff

gccincdir := $(shell $(CC) -print-file-name=include)

CFLAGS := $(DBGFLAGS) $(OPTFLAGS) $(ONEFLAGS)  \
	-D__KERNEL__ -DCONFIG_SYS_TEXT_BASE=0x80008000 -D__UBOOT__

CFLAGS += -I$(TOPDIR)includes/include -I$(TOPDIR)includes/arch/arm/include \
	    -include ./includes/include/linux/kconfig.h \
	    -fno-builtin -ffreestanding \
	    -fno-delete-null-pointer-checks -fstack-usage \
	    -fno-stack-protector\
	    -isystem $(gccincdir) -pipe $(PLATFORM_CPPFLAGS) 


ifdef BUILD_TAG
CFLAGS += $(TWOFLAGS) -Wall -Wstrict-prototypes \
	-DBUILD_TAG='"$(BUILD_TAG)"'
else
CFLAGS += $(TWOFLAGS) -Wall -Wstrict-prototypes
endif


# $(CPPFLAGS) sets -g, which causes gcc to pass a suitable -g<format>
# option to the assembler.
AFLAGS_DEBUG :=

AFLAGS := $(AFLAGS_DEBUG) $(CPPFLAGS)


#########################################################################

ELF 	= $(OUTPUTNAME)
BIN 	= $(OUTPUTNAME).bin
LIB = $(obj)libstubs.a
LIBCOBJS= stubs.o $(OUTPUTNAME).o
LIBOBJS	= $(addprefix $(obj),$(LIBCOBJS))

SRCS	:= $(COBJS:.o=.c) $(LIBCOBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))
ELF	:= $(addprefix $(obj),$(ELF))
BIN	:= $(addprefix $(obj),$(BIN))
SREC	:= $(addprefix $(obj),$(SREC))

gcclibdir := $(shell dirname `$(CC) -print-libgcc-file-name`)


all:	$(OBJS) $(LIB) $(BIN) $(ELF)	

clean:
		rm -f *.o *.a *.lst *.su .depend
		rm -f $(ELF) $(BIN)
		
#########################################################################
$(LIB):	$(obj).depend $(LIBOBJS)
		$(AR) $(ARFLAGS) $@ $(LIBOBJS)

$(OUTPUTNAME): $(OUTPUTNAME).o $(LIB)
		#linkage: -e is used to specify entry point
		$(LD) -g $(EX_LDFLAGS) -Ttext $(LOAD_ADDR) \
			-o $@ -e $(OUTPUTNAME) $< $(LIB)\
			-L$(gcclibdir) -lgcc

$(BIN):
$(obj)%.bin:	$(obj)%
		$(OBJCOPY) -O binary $< $@ 2>/dev/null


_depend:	$(obj).depend

$(obj).depend:	$(src)Makefile  $(SRCS)
		@rm -f $@
		@for f in $(SRCS); do \
			g=`basename $$f | sed -e 's/\(.*\)\.\w/\1.o/'`; \
			$(CC) -M $(HOST_CFLAGS) $(CFLAGS) -MQ $(obj)$$g $$f >> $@ ; \
		done

#########################################################################

%.s:	%.S
	$(CPP) $(AFLAGS) -o $@ $<
%.o:	%.S
	$(CC) $(AFLAGS) -c -o $@ $<
%.o:	%.c
	$(CC) $(CFLAGS) -c -Wa,-adhlns=$(@:.o=.lst) -o $@ $<

#########################################################################

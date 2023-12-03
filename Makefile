TARGET = umdidentifierV0.1
OBJS = main.o graphics.o framebuffer.o

CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBS = -lpspumd -lpspgu -lpng -lz -lm

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = UMD Identifier v0.1

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

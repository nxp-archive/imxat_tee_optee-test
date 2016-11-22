
CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf

OBJS = hello_world.o

CFLAGS += -Wall -I$(TEEC_EXPORT)/include
CFLAGS += -I../../ta/os_test/include
LDADD += -lteec -L$(TEEC_EXPORT)/lib

.PHONY: all
all: unsigned_hello

unsigned_hello:
	@echo "  ATP      $(O)/$@"
	$(q)mkdir -p $(O)
	$(q)$(CC) ${CFLAGS} $(LDADD) hello_world.c -o $(O)/$@

.PHONY: clean
clean:
	rm -f $(O)/unsigned_hello

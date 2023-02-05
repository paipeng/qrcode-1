CC      = gcc
#CFLAGS  = -Wall -Wextra -Werror -Wno-unused-but-set-variable -Wno-unused-variable -pedantic -ansi -std=c99 -O3
#CFLAGS  = -Wall -Wextra -Werror -Wno-unused-but-set-variable -Wno-unused-variable -std=c99 -O3
#LDFLAGS = -O3
TARGETS = libqrcode.a

INC_PATH = ./

CFLAGS += -std=c99
CFLAGS+= -I$(INC_PATH)
CFLAGS+= -DLINUX=1

ifeq ($(DEBUG),1)
        CFLAGS+= -DDEBUG=1 -g
else
        CFLAGS+= -O2
endif

SRCS = pbm.c encoder.c decoder.c rs.c bch.c blocks.c modules.c data.c


OBJ = $(patsubst %.c, %.o,$(SRCS))


%.o: %.c
	$(CC) -fPIC -c -o $@ $(patsubst %.o, %.c,$@)  $(CFLAGS)

main:static

static:	$(OBJ)
	ar cr $(TARGETS) $^
	rm -f *.o

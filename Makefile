SRCS=mgit.c objects.c subcommands.c mgit_path.c
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)
CC=gcc
CFLAGS=-Wall -Werror -g
TARGET=mgit
LIBS=-lssl -lcrypto

all: mgit
.PHONY: all

mgit: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(TARGET) $(OBJS)

clean:
	rm $(OBJS) $(DEPS) $(TARGET)
.PHONY: clean

%.o: %.c %.d
	$(CC) $(CFLAGS) -c $<

%.d: %.c 
	$(CC) -MM $< > $@

include $(DEPS)
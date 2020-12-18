SRCS=mgit.c db.c fs.c utils.c $(wildcard cmds/*.c)
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)
CC=gcc
CFLAGS=-Wall -Werror -g
TARGET=mgit
LIBS=-lssl -lcrypto

all: mgit
.PHONY: all

mgit: $(OBJS)
	echo $(SRCS)
	echo $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(TARGET) $(OBJS)

clean:
	rm $(OBJS) $(DEPS) $(TARGET)
.PHONY: clean

%.o: %.c %.d
	$(CC) $(CFLAGS) -c $< -o $@

%.d: %.c 
	$(CC) -MM $< > $@

include $(DEPS)
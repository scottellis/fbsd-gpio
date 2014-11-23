# Makefile for gpio example  

CC = cc
CFLAGS = -Wall

OBJS = main.o

TARGET = gpio 

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)

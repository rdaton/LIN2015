TARGET = chat

CC = gcc
CPPSYMBOLS=
CFLAGS = -g -Wall $(CPPSYMBOLS)
LDFLAGS = 

OBJS = chat.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET)  $(OBJS)

.c.o: 
	$(CC) $(CFLAGS)  -c  $<

clean: 
	-rm -f *.o $(TARGET) 

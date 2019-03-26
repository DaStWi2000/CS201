CC=gcc
CFLAGS= -std=c11 -c -g -pedantic-errors
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=portfolio
all: $(SOURCES) $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ -lm -lncurses
.c.o:
	$(CC) $(CFLAGS) $< -o $@
clean:
	@rm *.o
	@rm $(EXECUTABLE)

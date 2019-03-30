CC=gcc
CFLAGS= -std=c11 -c -g -Wall -pedantic-errors
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=portfolio
all: $(SOURCES) $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ -lm -lncurses
	@./build_database.sh
.c.o:
	$(CC) $(CFLAGS) $< -o $@
clean:
	@rm *.o
	@rm $(EXECUTABLE)
	@rm food_database.csv

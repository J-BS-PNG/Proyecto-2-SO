CC = gcc
CFLAGS = -Wall -Werror -g

TARGET = programa
SOURCES = pruebas.c
OBJECTS = $(SOURCES:.c=.o)

# Agrega aquí cualquier otra dependencia si es necesario
DEPENDENCIES = matriz.h listA.h sram.h util.h

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c $(DEPENDENCIES)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS)

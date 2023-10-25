# Nombre del ejecutable
TARGET = mmuSimulation

# Compilador
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -g `pkg-config --cflags gtk+-3.0`

# Bibliotecas
LIBS = `pkg-config --libs gtk+-3.0`

# Archivos fuente
SOURCES = mmuSimulationController.c
GLADE_FILE = mmuSimulation.glade
HEADERS = listA.h matriz.h sram.h svirtual.h util.h

# Objetos generados a partir de archivos fuente
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)

$(OBJECTS): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SOURCES)

clean:
	rm -f $(TARGET) $(OBJECTS)

run: $(TARGET)
	./$(TARGET) $(GLADE_FILE)

.PHONY: all clean run

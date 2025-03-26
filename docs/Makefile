# Definición del compilador de Emscripten
EMCC = emcc

# Opciones de compilación
CFLAGS = -O2 \
         -s WASM=1 \
         -lembind \
         -Iinclude \
         --preload-file data \
         -s INITIAL_MEMORY=134217728 \
         -s MAXIMUM_MEMORY=536870912 \
         -s ALLOW_MEMORY_GROWTH=1 \
         -finput-charset=UTF-8

# Archivos fuente: incluye .cpp y .cc, excluyendo Source.cpp si es necesario
SOURCES = $(filter-out src/Source.cpp, $(wildcard src/*.cpp) $(wildcard src/*.cc))

# Nombre del archivo de salida
TARGET = index.js

# Regla principal
all: $(TARGET)

# Regla para compilar el proyecto
$(TARGET): $(SOURCES)
	$(EMCC) $(SOURCES) $(CFLAGS) -o $(TARGET)

# Limpiar archivos generados
clean:
	rm -f $(TARGET) index.wasm index.data

.PHONY: all clean

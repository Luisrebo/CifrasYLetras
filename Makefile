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

# Obtención de todos los archivos fuente en los directorios 'src' y 'src2', excluyendo 'Source.cpp'
SOURCES = $(filter-out src/Source.cpp, $(wildcard src/*.cpp) $(wildcard src2/*.cpp))

# Nombre del archivo de salida
TARGET = index.js

# Regla principal
all: $(TARGET)

# Regla para compilar el proyecto
$(TARGET): $(SOURCES)
	$(EMCC) $(SOURCES) $(CFLAGS) -o $(TARGET)

# Regla para limpiar archivos generados
clean:
	rm -f $(TARGET) index.wasm index.data

# Declaración de objetivos que no son archivos
.PHONY: all clean

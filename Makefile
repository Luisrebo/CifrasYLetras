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

# Salida en la carpeta docs/
OUTDIR = docs
TARGET = $(OUTDIR)/index.js

# Regla principal
all: $(OUTDIR) $(TARGET)

# Asegura que exista docs/
$(OUTDIR):
	mkdir -p $(OUTDIR)

# Regla para compilar el proyecto dentro de docs/
$(TARGET): $(SOURCES)
	$(EMCC) $(SOURCES) $(CFLAGS) -o $(TARGET)

# Limpiar archivos generados en docs/
clean:
	rm -f $(TARGET) \
	       $(OUTDIR)/index.wasm \
	       $(OUTDIR)/index.data

.PHONY: all clean

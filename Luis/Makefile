EMCC = emcc 
CFLAGS = -O2 -s WASM=1 -lembind -Iinclude --preload-file data -s INITIAL_MEMORY=67108864 -s ALLOW_MEMORY_GROWTH=1 -finput-charset=UTF-8

# Filtramos Source.cpp de los archivos fuente
SOURCES = $(filter-out src/Source.cpp, $(wildcard src/*.cpp) $(wildcard src/*.cc))
TARGET = index.js

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(EMCC) $(SOURCES) $(CFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET) index.wasm index.data

.PHONY: all clean

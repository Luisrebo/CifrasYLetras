# ---- Compilador ----
EMCC := em++

# ---- Opciones ----
CFLAGS := -O3 -std=c++20 \
  -s WASM=1 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s INITIAL_MEMORY=134217728 \
  -s MAXIMUM_MEMORY=536870912 \
  -lembind \
  -ICommon/include \
  -INumbersChallenge/include \
  -ILettersChallenge/include \
  --preload-file data@/data \
  -finput-charset=UTF-8

# ---- Fuentes ----
SOURCES := \
  NumbersChallenge/src/BFS.cpp \
  NumbersChallenge/src/DFS.cpp \
  NumbersChallenge/src/Search.cpp \
  NumbersChallenge/src/SearchFactory.cpp \
  NumbersChallenge/src/SearchNumbers.cpp \
  NumbersChallenge/src/numbersChallenge.cpp \
  LettersChallenge/src/Trie.cpp \
  LettersChallenge/src/TrieSolver.cpp \
  LettersChallenge/src/AlphabeticalOrderHeuristic.cpp \
  LettersChallenge/src/HeightHeuristic.cpp \
  LettersChallenge/src/ProbabilityOrderHeuristic.cpp \
  LettersChallenge/src/ReachableWordsHeuristic.cpp \
  LettersChallenge/src/HeuristicFactory.cpp \
  LettersChallenge/src/IHeuristic.cpp \
  LettersChallenge/src/ProbabilityBuilder.cpp \
  LettersChallenge/src/lettersChallenge.cpp \
  Common/src/GlobalAverage.cpp \
  Common/src/commonFunctions.cpp \
  Bindings/bindings_mbind.cc

# ---- Salida ----
OUTDIR := docs
TARGET := $(OUTDIR)/index.js

# ---- Reglas ----
all: $(OUTDIR) $(TARGET)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(TARGET): $(SOURCES)
	$(EMCC) $(SOURCES) $(CFLAGS) -o $(TARGET)

clean:
	rm -f $(OUTDIR)/index.js \
	      $(OUTDIR)/index.wasm \
	      $(OUTDIR)/index.data

.PHONY: all clean

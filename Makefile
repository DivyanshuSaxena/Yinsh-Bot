IDIR =./include
CC = g++ -std=c++11
OPTI = -Ofast -march=native
CFLAGS = -I$(IDIR)
DEBUG = -g

SDIR =./src
BDIR =./build

_DEPS = Board.h State.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_CPP = Main.cpp Board.cpp State.cpp TimeHelper.cpp
CPP = $(patsubst %,$(SDIR)/%,$(_CPP))

_BUILDS = code
BUILDS = $(patsubst %,$(BDIR)/%,$(_BUILDS))

_GBUILDS = codeg
GBUILDS = $(patsubst %,$(BDIR)/%,$(_GBUILDS))

MKDIR = mkdir -p

OUT_DIR = bin build

all: directories $(BUILDS)

debug: directories $(GBUILDS)

directories: 
	$(MKDIR) $(OUT_DIR)

$(BDIR)/code: $(CPP)
	$(CC) $(OPTI) -o $@ $^ $(CFLAGS) 

$(BDIR)/codeg: $(CPP)
	$(CC) $(OPTI) -o $@ $^ $(CFLAGS) -g 

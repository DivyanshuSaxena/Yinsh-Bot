IDIR =./include
CC = g++ -std=c++11
OPTI = -Ofast
OPTI2 = -O2
OPTI3 = -O3
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

_BUILDS2 = code2
BUILDS2 = $(patsubst %,$(BDIR)/%,$(_BUILDS2))

_BUILDS3 = code3
BUILDS3 = $(patsubst %,$(BDIR)/%,$(_BUILDS3))

_GBUILDS = codeg
GBUILDS = $(patsubst %,$(BDIR)/%,$(_GBUILDS))

_NOBUILDS = nocode
NOBUILDS = $(patsubst %,$(BDIR)/%,$(_NOBUILDS))

MKDIR = mkdir -p

OUT_DIR = bin build

all: directories $(BUILDS)

debug: directories $(GBUILDS)

opti2: directories $(BUILDS2)

opti3: directories $(BUILDS3)

no-opti: directories $(NOBUILDS)

directories: 
	$(MKDIR) $(OUT_DIR)

$(BDIR)/code: $(CPP)
	$(CC) $(OPTI) -o $@ $^ $(CFLAGS)

$(BDIR)/code2: $(CPP)
	$(CC) $(OPTI2) -o $@ $^ $(CFLAGS) -g

$(BDIR)/code3: $(CPP)
	$(CC) $(OPTI3) -o $@ $^ $(CFLAGS) -g

$(BDIR)/codeg: $(CPP)
	$(CC) $(OPTI) -o $@ $^ $(CFLAGS) -g 

$(BDIR)/nocode: $(CPP)
	$(CC) -o $@ $^ $(CFLAGS) -g 

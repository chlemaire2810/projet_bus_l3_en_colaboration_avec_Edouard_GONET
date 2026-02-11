# Compiler and tool settings
CC            = gcc
# CFLAGS        = 
CFLAGS        = -march=native -mtune=native -pipe -fwhole-program -fPIE -flto=auto -fuse-linker-plugin -fno-fat-lto-objects -fno-plt -fgraphite -fgraphite-identity -floop-nest-optimize -fdevirtualize-at-ltrans
DEBUG_FLAGS   = -Og -g -Wall -Wextra -Wpedantic
RELEASE_FLAGS = -Ofast -s -DNDEBUG
SMOL_FLAGS    = -Oz -s -DNDEBUG
LIB_FLAGS     = -lm -lSDL2 -lSDL2_image -D_REENTRANT


# Default target
all: release debug smol

# Build the final executable
release:
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) *.c $(LIB_FLAGS) -o release

debug:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) *.c $(LIB_FLAGS) -o debug

smol:
	$(CC) $(CFLAGS) $(SMOL_FLAGS) *.c $(LIB_FLAGS) -o smol

# Clean up generated files
clean:
	rm -f *.o *.gch release debug smol mon_executable test

# Compiler to use
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra -std=c++17
DEBUG_FLAGS = -g
ASAN_FLAGS = -fsanitize=address -fno-omit-frame-pointer

# Linker flags (needed for ASan)
LDFLAGS = 
ASAN_LDFLAGS = -fsanitize=address

# Target executable name
TARGET = test

# Source file
SRC = test.cpp utility.cpp

# Object file
OBJ = $(SRC:.cpp=.o)

# External library directories
DOCTEST_DIR = external/doctest
FAKEIT_DIR = external/fakeit
NANOBENCH_DIR = external/nanobench
STB_DIR = external/stb
JSON_DIR = external/json

# Include paths for each library
INCLUDES = -I$(DOCTEST_DIR)/doctest/ \
           -I$(FAKEIT_DIR)/single_header/doctest \
           -I$(NANOBENCH_DIR)/src/include \
           -I$(STB_DIR) \
           -I$(JSON_DIR)/include/

# Default target: build without debug or ASan
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(TARGET)

# Compile source files to object files
%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Debug target: build with debugging symbols
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

# ASan target: build with AddressSanitizer and debugging symbols
asan: CFLAGS += $(DEBUG_FLAGS) $(ASAN_FLAGS)
asan: LDFLAGS += $(ASAN_LDFLAGS)
asan: $(TARGET)

# Initialize and update external libraries as submodules
external:
	git submodule add https://github.com/doctest/doctest.git $(DOCTEST_DIR) || true
	git submodule add https://github.com/eranpeer/FakeIt.git $(FAKEIT_DIR) || true
	git submodule add https://github.com/andreas-abel/nanoBench.git $(NANOBENCH_DIR) || true
	git submodule add https://github.com/nothings/stb.git $(STB_DIR) || true
	git submodule add https://github.com/nlohmann/json.git $(JSON_DIR) || true
	git submodule update --init --recursive

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)

# Phony targets (not actual files)
.PHONY: all debug asan external clean

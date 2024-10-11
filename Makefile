# Compiler
CC = gcc

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Target executable
TARGET = $(BUILD_DIR)/main

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# GTK4 flags
CFLAGS = $(shell pkg-config --cflags gtk4) -I$(INCLUDE_DIR)
LIBS = $(shell pkg-config --libs gtk4)

# Build target
$(TARGET): $(SRC_FILES)
	@$(CC) $(CFLAGS) -o $(TARGET) $(SRC_FILES) $(LIBS)

run: $(TARGET)
	@./$(TARGET)

# Clean up
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: clean run
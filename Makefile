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
LIBS = $(shell pkg-config --libs gtk4) -ljansson

# Build target
$(TARGET): $(SRC_FILES) | $(BUILD_DIR)
	@$(CC) $(CFLAGS) -o $(TARGET) $(SRC_FILES) $(LIBS)

# Ensure build directory exists
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

run: $(TARGET)
	@./$(TARGET)

bundle: $(TARGET)
	@./bundle_macos_app.sh
	@echo "Bundle created in build/"

# Clean up
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: clean run bundle
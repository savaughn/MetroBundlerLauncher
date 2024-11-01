# Compiler
CC = gcc

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
VERSION = 0.2.0

# Target executable
TARGET = $(BUILD_DIR)/main

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# GTK4 flags
CFLAGS = $(shell pkg-config --cflags gtk4 cairo jansson) -I$(INCLUDE_DIR)
LIBS = $(shell pkg-config --libs gtk4 cairo jansson)

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

dmg:
	@create-dmg --volname "Metro Bundler Launcher Installer" --window-pos 200 120 --window-size 800 400 --app-drop-link 600 185 "build/MetroBundlerLauncherInstaller-v$(VERSION).dmg" "build/"
	@echo "DMG created in build/"

# Clean up
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: clean run bundle
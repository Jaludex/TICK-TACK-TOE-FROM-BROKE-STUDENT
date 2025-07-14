# =============================================
# Basic Configuration
# =============================================
SRC_DIR := lib
BUILD_DIR := build
BIN_DIR := bin
TARGET ?= linux

# Sources
SRCS := $(wildcard $(SRC_DIR)/*.cpp) main.cpp
OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SRCS)))

# =============================================
# Platform-Specific Configuration
# =============================================
ifeq ($(TARGET), windows)
    # Windows cross-compilation
    CXX := x86_64-w64-mingw32-g++
    EXEC := $(BIN_DIR)/main.exe
    LDFLAGS := -static -lstdc++ -lwinpthread -lgcc -lmingw32 -lmingwex
    CXXFLAGS := -std=c++17 -Wall -Wextra -I$(SRC_DIR) -D_GLIBCXX_USE_CXX11_ABI=0
else
    # Native Linux compilation
    CXX := clang++
    EXEC := $(BIN_DIR)/main
    LDFLAGS := -lstdc++ -pthread -lm
    CXXFLAGS := -std=c++17 -Wall -Wextra -I$(SRC_DIR)
endif

# =============================================
# AppImage Configuration
# =============================================
APP_NAME := TicTacToe
APP_VERSION := 1.0
APP_IMAGE := $(BIN_DIR)/$(APP_NAME)-$(APP_VERSION)-x86_64.AppImage  # Changed to bin directory

.PHONY: all clean win linux appimage

# =============================================
# Main Targets
# =============================================
all: $(EXEC)

win:
	@$(MAKE) TARGET=windows

linux:
	@$(MAKE) TARGET=linux

# =============================================
# Build Rules
# =============================================
$(EXEC): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	chmod +x $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: main.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $@

$(BIN_DIR):
	@mkdir -p $@

# =============================================
# Clean Target (preserves bin directory)
# =============================================
clean:
	@rm -rf $(BUILD_DIR) *.AppImage TicTacToe.AppDir squashfs-root
	@echo "Build artifacts cleaned"

# =============================================
# AppImage Target
# =============================================
appimage: linux
	@echo "Creating AppDir structure..."
	@mkdir -p TicTacToe.AppDir/usr/bin
	@mkdir -p TicTacToe.AppDir/usr/lib
	@cp $(BIN_DIR)/main TicTacToe.AppDir/usr/bin/tictactoe
	
	# Copy required libraries
	@ldd $(BIN_DIR)/main | awk '$$3 ~ /^\// {print $$3}' | sort -u | while read lib; do \
		cp -Lv "$$lib" TicTacToe.AppDir/usr/lib/; \
	done
	
	# Copy application icon
	@cp assets/icon.png TicTacToe.AppDir/tictactoe.png
	
	# Create .desktop file
	@echo "[Desktop Entry]" > TicTacToe.AppDir/tictactoe.desktop
	@echo "Name=Tic Tac Toe" >> TicTacToe.AppDir/tictactoe.desktop
	@echo "Exec=tictactoe" >> TicTacToe.AppDir/tictactoe.desktop
	@echo "Icon=tictactoe" >> TicTacToe.AppDir/tictactoe.desktop
	@echo "Type=Application" >> TicTacToe.AppDir/tictactoe.desktop
	@echo "Categories=Game;" >> TicTacToe.AppDir/tictactoe.desktop
	@echo "Comment=A classic Tic Tac Toe game" >> TicTacToe.AppDir/tictactoe.desktop
	
	# Create AppRun launcher
	@echo "#!/bin/sh" > TicTacToe.AppDir/AppRun
	@echo 'HERE="$$(dirname "$$(readlink -f "$$0")")"' >> TicTacToe.AppDir/AppRun
	@echo 'export LD_LIBRARY_PATH="$${HERE}/usr/lib:$${LD_LIBRARY_PATH}"' >> TicTacToe.AppDir/AppRun
	@echo 'cd "$${HERE}/usr/bin"' >> TicTacToe.AppDir/AppRun
	@echo 'exec "./tictactoe" "$$@"' >> TicTacToe.AppDir/AppRun
	@chmod +x TicTacToe.AppDir/AppRun
	
	@echo "Downloading appimagetool..."
	@wget -q -nc https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
	@chmod +x appimagetool-x86_64.AppImage
	
	@echo "Generating AppImage..."
	@./appimagetool-x86_64.AppImage --appimage-extract >/dev/null
	@./squashfs-root/AppRun TicTacToe.AppDir
	
	@echo "Moving AppImage to bin directory"
	@mkdir -p $(BIN_DIR)
	@mv Tic_Tac_Toe-x86_64.AppImage $(APP_IMAGE)
	@chmod +x $(APP_IMAGE)
	@echo "AppImage created: $(APP_IMAGE)"
# Configuración básica
SRC_DIR := lib
BUILD_DIR := build
BIN_DIR := bin
TARGET ?= linux

# Lista de archivos fuente
SRCS := $(wildcard $(SRC_DIR)/*.cpp) main.cpp
OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SRCS)))

# Configuración específica por plataforma
ifeq ($(TARGET), windows)
    # Configuración para Windows
    CXX := x86_64-w64-mingw32-g++
    EXEC := $(BIN_DIR)/main.exe
    LDFLAGS := -static -lstdc++ -lwinpthread -lgcc -lmingw32 -lmingwex
    CXXFLAGS := -std=c++17 -Wall -Wextra -I$(SRC_DIR) -D_GLIBCXX_USE_CXX11_ABI=0
else
    # Configuración para Linux
    CXX := clang++
    EXEC := $(BIN_DIR)/main
    LDFLAGS := -lstdc++ -pthread -lm
    CXXFLAGS := -std=c++17 -Wall -Wextra -I$(SRC_DIR)
endif

.PHONY: all clean win linux

all: $(EXEC)

win:
	@$(MAKE) TARGET=windows

linux:
	@$(MAKE) TARGET=linux

$(EXEC): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: main.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $@

$(BIN_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
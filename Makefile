## Cross-platform Makefile for AutoTyper

# Phony targets
.PHONY: all clean release debug

# OS detection

ifeq ($(OS),Windows_NT)
    $(info $(OS): Running on Windows)
    PLATFORM = WINDOWS
else
    $(info $(OS): Running on Unix-like system)
    PLATFORM = LINUX
endif

# Variables
CXX = g++
CXX_FLAGS = -Wall -Os -std=c++17 -ffunction-sections -fdata-sections # -fno-rtti -fomit-frame-pointer
LD_FLAGS += -s -Wl,--gc-sections # -Wl,-strip-all

# Platform-specific settings
ifeq ($(PLATFORM),WINDOWS)
    LIBS = -lUser32 -lImm32
    TARGET = AutoTyper.exe
    RM = del /f /q
    CLEAN_FILES = src\*.o 
else
    LIBS = -lX11 -lXtst
    TARGET = AutoTyper
    RM = rm -f
    CLEAN_FILES = src/*.o src/**/*.o 
endif

# Directories & Files
SRC_DIR = src
SRC_FILES := $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp)
HDR_FILES := $(wildcard $(SRC_DIR)/**/*.h $(SRC_DIR)/*.h)
OBJ_FILES := $(patsubst %.cpp,%.o,$(SRC_FILES))

# Default target
all: $(TARGET)
	@echo Build on $(PLATFORM)

# Link target
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXX_FLAGS) $(LD_FLAGS) -o $@ $^ $(LIBS)

# Compile source code to object files
%.o: %.cpp $(HDR_FILES)
	$(CXX) $(CXX_FLAGS) -c $< -o $@

# Clean target
clean:
	$(RM) $(CLEAN_FILES)

cleanall:
	$(RM) $(CLEAN_FILES) $(TARGET)

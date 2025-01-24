## Build AutoTyper on Linux

# Phony targets
.PHONY: all clean

# Variables
CXX = g++
CXX_FLAGS = -Wall -O2
LIBS = -lX11 -lXtst
TARGET = AutoTyper_linux
SRC = AutoTyper_linux.cpp

# Default target
all: $(TARGET)

# Compile and link in one step
$(TARGET): $(SRC)
	$(CXX) $(CXX_FLAGS) -o $@ $< $(LIBS)

# Clean target
clean:
	rm -f $(TARGET)

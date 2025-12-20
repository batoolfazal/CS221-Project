# Makefile for Drone Telemetry Feature

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
TARGET = drone_telemetry

# Source files
SOURCES = $(SRC_DIR)/main_telemetry.cpp \
          $(SRC_DIR)/DroneLogger.cpp \
          $(SRC_DIR)/WaypointBuffer.cpp \
          $(SRC_DIR)/FlightHashMap.cpp \
          $(SRC_DIR)/ErrorLog.cpp \
          $(SRC_DIR)/Quadcopter.cpp \
          $(SRC_DIR)/GPSData.cpp \
          $(SRC_DIR)/IMUData.cpp \
          $(SRC_DIR)/MotorControl.cpp

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo "✅ Build successful: $(TARGET)"

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Generate flight data
generate:
	python3 GenerateDroneFlightData.py

# Run the program
run: $(TARGET) generate
	./$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET) DroneFlightLog.csv drone_errors.log
	@echo "🧹 Cleaned build files"

# Clean only generated data
clean-data:
	rm -f DroneFlightLog.csv drone_errors.log
	@echo "🧹 Cleaned data files"

# Rebuild everything
rebuild: clean all

# Help
help:
	@echo "Available targets:"
	@echo "  make          - Build the project"
	@echo "  make generate - Generate flight data (Python)"
	@echo "  make run      - Generate data and run program"
	@echo "  make clean    - Remove all build files and data"
	@echo "  make clean-data - Remove only generated data files"
	@echo "  make rebuild  - Clean and rebuild"
	@echo "  make help     - Show this help message"

.PHONY: all clean clean-data rebuild generate run help


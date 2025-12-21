# Makefile for Telemetry Logger Feature

CXX = g++
CXXFLAGS = -std=c++11 -Wall -I.
TARGET = drone_telemetry

# Source files
SOURCES = main_telemetry.cpp \
          DroneLogger.cpp \
          WaypointBuffer.cpp \
          FlightHashMap.cpp \
          ErrorLog.cpp \
          Quadcopter.cpp \
          GPSData.cpp \
          IMUData.cpp \
          MotorControl.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)
	@echo "✅ Build successful: $(TARGET)"

# Build executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Compile objects
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Generate flight data
generate:
	python3 GenerateDroneFlightData.py
	@echo "✅ Generated DroneFlightLog.csv"

# Run the program
run: $(TARGET) generate
	./$(TARGET)

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET) DroneFlightLog.csv drone_errors.log
	@echo "🧹 Cleaned build files"

# Help
help:
	@echo "Available targets:"
	@echo "  make          - Build the drone_telemetry executable"
	@echo "  make generate - Run Python script to create DroneFlightLog.csv"
	@echo "  make run      - Build, generate data, and run"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make help     - Show this help message"

.PHONY: all generate run clean help


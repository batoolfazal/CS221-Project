#include <iostream>
#include <vector>

// Cross-platform sleep
#ifdef _WIN32
#include <windows.h>
#define sleep_for(ms) Sleep(ms)
#else
#include <unistd.h>
#define sleep_for(ms) usleep((ms) * 1000)
#endif

// Only 3 master includes
#include "TelemetryLogger.h"
#include "DroneControl.h"
#include "DroneSensors.h"

int main() {
    std::cout << "\n============================================\n";
    std::cout << "   DRONE TELEMETRY SYSTEM - D-1 FEATURE\n";
    std::cout << "============================================\n\n";
    
    ErrorLog errorLog;
    errorLog.info("System startup initiated");
    
    DroneLogger logger("DroneFlightLog.csv");
    logger.ReadFromFile();
    errorLog.info("Flight data loaded successfully");
    
    std::vector<FlightData> last10 = logger.getLastNReadings(10);
    
    std::cout << "\n=== Last 10 Flight Readings ===\n";
    for (size_t i = 0; i < last10.size(); i++) {
        std::cout << "[" << i << "] Alt: " << last10[i].altitude << "m, "
             << "Battery: " << last10[i].battery << "%\n";
    }
    
    QuadcopterSystem drone(4);
    drone.armMotors();
    
    WaypointBuffer buffer;
    for (auto& data : last10) {
        buffer.Insert(data.latitude, data.longitude, data.altitude);
    }
    buffer.Display();
    
    std::cout << "\n=== FLIGHT SIMULATION ===\n";
    
    bool landed = false;
    int iteration = 0;
    FlightHashMap flightCache;
    
    while (!landed && iteration < static_cast<int>(last10.size())) {
        FlightData& current = last10[iteration];
        
        std::cout << "Iteration " << iteration << ":\n";
        std::cout << "  Alt: " << current.altitude << "m, Battery: " 
             << current.battery << "%\n";
        
        drone.stabilize(current.roll, current.pitch);
        
        std::string key = "T_" + std::to_string(iteration);
        flightCache.insert(key, "Alt:" + std::to_string(current.altitude));
        
        if (current.altitude < 1.0 || current.battery < 25.0) {
            landed = true;
            std::cout << "🛬 Landing...\n";
        }
        
        sleep_for(500);
        iteration++;
    }
    
    drone.disarmMotors();
    
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "STATISTICS\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "Max Altitude: " << logger.MaxAltitude() << "m\n";
    std::cout << "Avg Velocity: " << logger.AverageVelocity() << " m/s\n";
    std::cout << "Total Distance: " << logger.TotalDistance() << "m\n";
    std::cout << std::string(60, '=') << "\n";
    
    logger.DisplayReverse(5);
    
    errorLog.info("Mission completed");
    std::cout << "\n✅ Complete!\n\n";
    
    return 0;
}


#include "ErrorLog.h"
#include <iostream>
#include <ctime>

std::string ErrorLog::getCurrentTime() {
    time_t now = time(nullptr);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buffer);
}

ErrorLog::ErrorLog(std::string fname) : filename(fname) {
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Warning: Cannot open error log file\n";
    }
}

ErrorLog::~ErrorLog() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void ErrorLog::warning(std::string message) {
    std::string timestamp = getCurrentTime();
    std::string logEntry = "[" + timestamp + "] [WARNING] " + message;
    
    std::cout << "⚠️  " << logEntry << std::endl;
    
    if (logFile.is_open()) {
        logFile << logEntry << std::endl;
        logFile.flush();
    }
}

void ErrorLog::critical(std::string message) {
    std::string timestamp = getCurrentTime();
    std::string logEntry = "[" + timestamp + "] [CRITICAL] " + message;
    
    std::cout << "🚨 " << logEntry << std::endl;
    
    if (logFile.is_open()) {
        logFile << logEntry << std::endl;
        logFile.flush();
    }
}

void ErrorLog::info(std::string message) {
    std::string timestamp = getCurrentTime();
    std::string logEntry = "[" + timestamp + "] [INFO] " + message;
    
    std::cout << "ℹ️  " << logEntry << std::endl;
    
    if (logFile.is_open()) {
        logFile << logEntry << std::endl;
        logFile.flush();
    }
}


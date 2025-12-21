#ifndef ERROR_LOG_H
#define ERROR_LOG_H

#include <fstream>
#include <string>

// Error logging to file and console
class ErrorLog {
private:
    std::ofstream logFile;
    std::string filename;
    
    std::string getCurrentTime();
    
public:
    ErrorLog(std::string fname = "drone_errors.log");
    ~ErrorLog();
    
    void warning(std::string message);
    void critical(std::string message);
    void info(std::string message);
};

#endif // ERROR_LOG_H


#ifndef FLIGHT_HASHMAP_H
#define FLIGHT_HASHMAP_H

#include <unordered_map>
#include <string>

// Fast retrieval using hash map
class FlightHashMap {
private:
    std::unordered_map<std::string, std::string> dataMap;
    
public:
    FlightHashMap();
    
    void insert(std::string key, std::string value);
    std::string get(std::string key);
    bool exists(std::string key);
    void displayAll();
    
    int size() const;
};

#endif // FLIGHT_HASHMAP_H


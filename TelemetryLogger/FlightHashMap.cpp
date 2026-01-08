#include "FlightHashMap.h"
#include <iostream>

FlightHashMap::FlightHashMap() {
    std::cout << "🗂️  FlightHashMap initialized\n";
}

void FlightHashMap::insert(std::string key, std::string value) {
    dataMap[key] = value;
}

std::string FlightHashMap::get(std::string key) {
    if (dataMap.find(key) != dataMap.end()) {
        return dataMap[key];
    }
    return "";
}

bool FlightHashMap::exists(std::string key) {
    return dataMap.find(key) != dataMap.end();
}

void FlightHashMap::displayAll() {
    std::cout << "\n=== FlightHashMap Contents ===\n";
    for (auto& pair : dataMap) {
        std::cout << pair.first << " => " << pair.second << "\n";
    }
    std::cout << "\n";
}

int FlightHashMap::size() const {
    return dataMap.size();
}


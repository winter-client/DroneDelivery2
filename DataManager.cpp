// DataManager.cpp
#include "DataManager.h"

DataManager::DataManager(const std::string& id, const std::string& destination, int hours, int minutes)
    : id(id), destination(destination), timing(hours, minutes) {}

DataManager::~DataManager() {}

const std::string& DataManager::getId() const {
    return id;
}

const std::string& DataManager::getDestination() const {
    return destination;
}

const Time& DataManager::getTiming() const {
    return timing;
}

void DataManager::setId(const std::string& newId) {
    id = newId;
}

void DataManager::setDestination(const std::string& newDestination) {
    destination = newDestination;
}

void DataManager::setTiming(int hours, int minutes) {
    timing = Time(hours, minutes);
}
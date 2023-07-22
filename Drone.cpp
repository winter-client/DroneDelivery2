// Drone.cpp
#include "Drone.h"
#include <iostream>

Drone::Drone(const std::string& id, const std::string& destination, int hours, int minutes, CapacityType capacityType)
    : DataManager(id, destination, hours, minutes), capacityType(capacityType) {}

void Drone::display() const {
    std::string capacity;
    switch (capacityType) {
    case CapacityType::MINI:
        capacity = "Mini (2)";
        break;
    case CapacityType::MAXI:
        capacity = "Maxi (5)";
        break;
    case CapacityType::HEAVY:
        capacity = "Heavy (10)";
        break;
    default:
        capacity = "Unknown";
        break;
    }

    std::cout << "Drone ID: " << getId() << ", Destination: " << getDestination()
        << ", Timing: " << getTiming().toString() << ", Capacity: " << capacity << " kg" << std::endl;
}

CapacityType Drone::getCapacityType() const {
    return capacityType;
}
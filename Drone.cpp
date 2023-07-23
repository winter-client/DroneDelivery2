// Drone.cpp


#include <iostream>

using namespace std;

#include "Drone.h"

Drone::Drone(const string& id, const string& destination, int hours, int minutes, CapacityType capacityType)
    : DataManager(id, destination, hours, minutes), capacityType(capacityType) {}

void Drone::display() const {
    string capacity;
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

    cout << "Drone ID: " << getId() << ", Destination: " << getDestination()
        << ", Timing: " << getTiming().toString() << ", Capacity: " << capacity  << endl;
}

CapacityType Drone::getCapacityType() const {
    return capacityType;
}
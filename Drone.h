// Drone.h

#ifndef DRONE_H
#define DRONE_H

#include "DataManager.h"

enum class CapacityType {
    MINI,
    MAXI,
    HEAVY
};

class Drone : public DataManager {
private:
    CapacityType capacityType;
    // Add more drone-specific attributes if needed
public:
    Drone(const std::string& id, const std::string& destination, int hours, int minutes, CapacityType capacityType);
    Drone() : DataManager("", "", 0, 0), capacityType(CapacityType::MINI) {}

    void display() const override;

    // Add a getter function for capacityType
    CapacityType getCapacityType() const;

    // Declare DataManager as a friend class to access private members
    friend class DataManager;
};

#endif // DRONE_H
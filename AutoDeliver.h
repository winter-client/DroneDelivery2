// AutoDeliver.h

#ifndef AUTODELIVER_H
#define AUTODELIVER_H

#include <vector>
#include <algorithm>
#include <unordered_map>


using namespace std;

#include "DataManager.h"
#include "Drone.h"
#include "Package.h"

enum class WriteMode {
    Append,
    Truncate,
    Edit
};

class AutoDeliver {
private:
    std::vector<Drone> drones;
    std::vector<Package> packages;

public:
    AutoDeliver();

    void readFiles(const string& droneFilePath, const string& packageFilePath);

    // Helper functions to write data to files
    void writeDronesToFile(const string& droneFilePath, WriteMode mode);
    void writePackagesToFile(const string& packageFilePath, WriteMode mode);

    // methods to allow add/edit/delete operations
    void addDrone(const Drone& newDrone);
    void editDrone(const string& droneId, const Drone& updatedDrone);
    void deleteDrone(const string& droneId);

    void addPackage(const Package& newPackage);
    void editPackage(const string& packageId, const Package& updatedPackage);
    void deletePackage(const string& packageId);

    void generateMatchingPlan_OptimizeCapacity();
    void generateMatchingPlan_MinimiseDrones();

    void displayDrones_NotAtFullLoadCapacity();
    void displayPackages_NotAssignedToDrone();

    void saveMatchingPlan(const string& outputFilePath);
};

#endif // AUTODELIVER_H

// AutoDeliver.cpp
#include <fstream>
#include <iostream>

using namespace std;

#include "AutoDeliver.h"

AutoDeliver::AutoDeliver() {}

void AutoDeliver::readFiles(const string& droneFilePath, const string& packageFilePath) {
    // Clear existing vectors before populating them
    drones.clear();
    packages.clear();

    // Read drone data from the file and populate the drones vector
    ifstream droneFile(droneFilePath);
    if (!droneFile.is_open()) {
        cerr << "Error: Unable to open the drone data file." << endl;
        return;
    }

    string droneId, destination;
    int hours, minutes;
    string capacityTypeStr;

    while (droneFile >> droneId >> destination >> hours >> minutes >> capacityTypeStr) {
        CapacityType capacityType;
        if (capacityTypeStr == "MINI") {
            capacityType = CapacityType::MINI;
        }
        else if (capacityTypeStr == "MAXI") {
            capacityType = CapacityType::MAXI;
        }
        else if (capacityTypeStr == "HEAVY") {
            capacityType = CapacityType::HEAVY;
        }
        else {
            cerr << "Warning: Invalid capacity type for drone " << droneId << endl;
            capacityType = CapacityType::MINI; // Default to MINI if capacity type is unknown
        }

        Drone newDrone(droneId, destination, hours, minutes, capacityType);
        drones.push_back(newDrone);

        // Debug output for the added drone
        cout << "Added Drone: ID=" << newDrone.getId() << ", Destination=" << newDrone.getDestination()
            << ", Time=" << newDrone.getTiming().toString() << ", Capacity=" << static_cast<int>(newDrone.getCapacityType()) << endl;
    }

    droneFile.close();

    // Read package data from the file and populate the packages vector
    ifstream packageFile(packageFilePath);
    if (!packageFile.is_open()) {
        cerr << "Error: Unable to open the package data file." << endl;
        return;
    }

    string packageId;
    string packageDestination;
    int packageHours, packageMinutes;

    while (packageFile >> packageId >> packageDestination >> packageHours >> packageMinutes) {
        Package newPackage(packageId, packageDestination, packageHours, packageMinutes);
        packages.push_back(newPackage);

        // Debug output for the added package
        cout << "Added Package: ID=" << newPackage.getId() << ", Destination=" << newPackage.getDestination()
            << ", Time=" << newPackage.getTiming().toString() << endl;
    }

    packageFile.close();
}


void AutoDeliver::generateMatchingPlan_OptimizeCapacity(const std::string& outputFilePath) {
    // Sort drones by capacity in descending order (heaviest to lightest)
    sort(drones.begin(), drones.end(), [](const Drone& a, const Drone& b) {
        return a.getCapacityType() > b.getCapacityType();
        });

    // Create a map to store the assigned package count for each drone
    std::unordered_map<std::string, int> droneAssignedCount;
    for (const auto& drone : drones) {
        droneAssignedCount[drone.getId()] = 0;
    }

    // Create a map to store the matching of packages to drones
    std::unordered_map<std::string, std::string> packageToDroneMap;

    // Create a vector to track unassigned packages
    std::vector<Package> unassignedPackages = packages;

    // Iterate through packages and assign them to drones based on capacity
    for (const auto& package : packages) {
        for (auto& drone : drones) {
            if (drone.getDestination() == package.getDestination() &&
                droneAssignedCount[drone.getId()] < static_cast<int>(drone.getCapacityType())) {
                // Assign the package to the drone
                packageToDroneMap[package.getId()] = drone.getId();
                droneAssignedCount[drone.getId()]++;

                // Mark the package as assigned
                auto it = std::find_if(unassignedPackages.begin(), unassignedPackages.end(),
                    [&](const Package& p) {
                        return p.getId() == package.getId();
                    });
                if (it != unassignedPackages.end()) {
                    unassignedPackages.erase(it);
                }

                // Exit the inner loop and go to the next package
                break;
            }
        }
    }

    // Display the matching plan
    for (const auto& pair : packageToDroneMap) {
        std::cout << "Drone: " << pair.second << " -> Package: " << pair.first << std::endl;
    }
}



void AutoDeliver::generateMatchingPlan_MinimiseDrones() {
    // Create a map to store the best match for each package
    unordered_map<string, Drone> packageToDroneMap;

    for (const auto& package : packages) {
        // Find the first drone with matching destination and arrival window
        auto it = std::find_if(drones.begin(), drones.end(), [&](const Drone& drone) {
            int droneTotalMinutes = drone.getTiming().getTotalMinutes();
            int packageTotalMinutes = package.getTiming().getTotalMinutes();
            return drone.getDestination() == package.getDestination() &&
                (droneTotalMinutes <= packageTotalMinutes && packageTotalMinutes - droneTotalMinutes <= 20);
            });

        if (it != drones.end()) {
            packageToDroneMap[package.getId()] = *it;
            // Do not remove the selected drone from the available drones to minimize the number of drones used.
        }
    }

    // Display the matching plan
    for (const auto& pair : packageToDroneMap) {
        const Drone& drone = pair.second;
        const Package& package = packages[std::distance(packages.begin(), std::find_if(packages.begin(), packages.end(),
            [&](const Package& p) {
                return p.getId() == pair.first;
            }))];

        std::cout << "Drone: " << drone.getId() << " -> Package: " << package.getId() << std::endl;
    }
}


void AutoDeliver::displayDrones_NotAtFullLoadCapacity() {
    std::cout << "========== Drones that are not at full load capacity ==========" << std::endl;
    for (const auto& drone : drones) {
        int currentLoadCapacity = 0;
        for (const auto& package : packages) {
            if (package.getDestination() == drone.getDestination()) {
                currentLoadCapacity++;
            }
        }

        int maxCapacity = 0;
        switch (drone.getCapacityType()) {
        case CapacityType::MINI:
            maxCapacity = 2;
            break;
        case CapacityType::MAXI:
            maxCapacity = 5;
            break;
        case CapacityType::HEAVY:
            maxCapacity = 10;
            break;
        default:
            break;
        }

        if (currentLoadCapacity < maxCapacity) {
            std::cout << "Drone ID: " << drone.getId() << ", Destination: " << drone.getDestination()
                << ", Timing: " << drone.getTiming().toString() << ", Capacity: " << maxCapacity  << std::endl;
        }
    }
}

void AutoDeliver::displayPackages_NotAssignedToDrone() {
    cout << "========== Packages that are not assigned to any drone ==========" << endl;
    for (const auto& package : unassignedPackages) { // Use unassignedPackages instead of packages
        bool isPackageAssigned = false;
        for (const auto& drone : drones) {
            if (package.getDestination() == drone.getDestination()) {
                isPackageAssigned = true;
                break;
            }
        }

        if (!isPackageAssigned) {
            cout << "Package ID: " << package.getId() << ", Destination: " << package.getDestination()
                << ", Timing: " << package.getTiming().toString() << endl;
        }
    }
}


void AutoDeliver::saveMatchingPlan(const std::string& outputFilePath) {
    ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        cout << "Error: Unable to open the output file." << endl;
        return;
    }

    // Write the matching plan to the output file
    for (const auto& drone : drones) {
        outputFile << "Drone: " << drone.getId() << " -> Package: ";
        bool packageAssigned = false;
        for (const auto& package : packages) {
            if (drone.getDestination() == package.getDestination()) {
                outputFile << package.getId() << ", ";
                packageAssigned = true;
                break;
            }
        }
        if (!packageAssigned) {
            outputFile << "No package assigned, ";
        }
        outputFile << endl;
    }

    outputFile.close();
}


void AutoDeliver::writeDronesToFile(const string& droneFilePath, WriteMode mode) {
    std::ofstream droneFile;

    switch (mode) {
    case WriteMode::Append:
        droneFile.open(droneFilePath, ios::app); // Open the file in append mode
        break;
    case WriteMode::Truncate:
        droneFile.open(droneFilePath, ios::trunc); // Open the file in truncate mode
        break;
    default:
        cerr << "Error: Invalid write mode specified." << endl;
        return;
    }

    if (!droneFile.is_open()) {
        cerr << "Error: Unable to open the drone data file." << endl;
        return;
    }

    // Write the drone data to the output file
    for (const auto& drone : drones) {
        droneFile << drone.getId() << " " << drone.getDestination() << " "
            << drone.getTiming().toString() << " ";

        switch (drone.getCapacityType()) {
        case CapacityType::MINI:
            droneFile << "2";
            break;
        case CapacityType::MAXI:
            droneFile << "5";
            break;
        case CapacityType::HEAVY:
            droneFile << "10";
            break;
        default:
            droneFile << "Unknown";
            break;
        }
        droneFile << endl;
    }

    droneFile.close();
}

void AutoDeliver::writePackagesToFile(const string& packageFilePath, WriteMode mode) {
    std::ofstream packageFile;

    switch (mode) {
    case WriteMode::Append:
        packageFile.open(packageFilePath, ios::app); // Open the file in append mode
        break;
    case WriteMode::Truncate:
        packageFile.open(packageFilePath, ios::trunc); // Open the file in truncate mode
        break;
    default:
        cerr << "Error: Invalid write mode specified." << endl;
        return;
    }

    if (!packageFile.is_open()) {
        cerr << "Error: Unable to open the package data file." << endl;
        return;
    }

    // Write the package data to the output file
    for (const auto& package : packages) {
        packageFile << package.getId() << " " << package.getDestination() << " "
            << package.getTiming().toString() << endl;
    }

    packageFile.close();
}


void AutoDeliver::addDrone(const Drone& newDrone) {
    drones.push_back(newDrone);
    writeDronesToFile("drones.txt", WriteMode::Append); // Save changes to the drone file
}

void AutoDeliver::editDrone(const string& droneIdToEdit, const Drone& newDrone) {
    // Search for the drone to be edited in the vector
    auto it = find_if(drones.begin(), drones.end(), [&](const Drone& drone) {
        return drone.getId() == droneIdToEdit;
        });

    if (it != drones.end()) {
        // Modify the existing drone data with the new drone data
        *it = newDrone;

        // Save the updated data to the file in truncate mode to overwrite existing data
        writeDronesToFile("drones.txt", WriteMode::Truncate);
        cout << "Drone with ID " << droneIdToEdit << " has been edited successfully." << endl;
    }
    else {
        cerr << "Error: Drone with ID " << droneIdToEdit << " not found." << endl;
    }
}

void AutoDeliver::deleteDrone(const std::string& droneId) {
    // Find the drone with the given ID in the vector
    auto it = find_if(drones.begin(), drones.end(), [&](const Drone& drone) {
        return drone.getId() == droneId;
        });

    if (it != drones.end()) {
        // Remove the drone from the vector
        drones.erase(it);
        cout << "Drone with ID: " << droneId << " deleted successfully." << endl;
        // Update the file after deleting the drone from the vector
        writeDronesToFile("drones.txt", WriteMode::Truncate);
    }
    else {
        cerr << "Error: Drone with ID: " << droneId << " not found." << endl;
    }
}


void AutoDeliver::addPackage(const Package& newPackage) {
    packages.push_back(newPackage);
    unassignedPackages.push_back(newPackage);
    writePackagesToFile("packages.txt", WriteMode::Append); // Save changes to the package file
}

void AutoDeliver::editPackage(const string& packageIdToEdit, const Package& newPackage) {
    // Search for the package to be edited in the vector
    auto it = find_if(packages.begin(), packages.end(), [&](const Package& package) {
        return package.getId() == packageIdToEdit;
        });

    if (it != packages.end()) {
        // Modify the existing package data with the new package data
        *it = newPackage;

        // Save the updated data to the file in truncate mode to overwrite existing data
        writePackagesToFile("packages.txt", WriteMode::Truncate);
        cout << "Package with ID " << packageIdToEdit << " has been edited successfully." << endl;
    }
    else {
        cerr << "Error: Package with ID " << packageIdToEdit << " not found." << endl;
    }
}

void AutoDeliver::deletePackage(const string& packageIdToDelete) {
    // Search for the package to be deleted in the vector
    auto it = find_if(packages.begin(), packages.end(), [&](const Package& package) {
        return package.getId() == packageIdToDelete;
        });

    if (it != packages.end()) {
        // Remove the package from the vector
        packages.erase(it);

        // Save the updated data to the file in truncate mode to overwrite existing data
        writePackagesToFile("packages.txt", WriteMode::Truncate);
        cout << "Package with ID " << packageIdToDelete << " has been deleted successfully." << endl;
    }
    else {
        cerr << "Error: Package with ID " << packageIdToDelete << " not found." << endl;
    }
}
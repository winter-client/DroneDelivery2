#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "UserInterface.h"

void UserInterface::start() {
    AutoDeliver autoDeliver;

    string droneFilePath, packageFilePath;

    cout << "Enter the file path for drones: ";
    cin >> droneFilePath;
    if (!isPathValid(droneFilePath) || !fileExists(droneFilePath)) {
        std::cerr << "Error: Invalid or not found drones.txt file." << endl;
        return;
    }

    cout << "Enter the file path for packages: ";
    cin >> packageFilePath;
    if (!isPathValid(packageFilePath) || !fileExists(packageFilePath)) {
        cerr << "Error: Invalid or not found packages.txt file." << endl;
        return;
    }

    // Load data from files
    autoDeliver.readFiles(droneFilePath, packageFilePath);

    // User interface loop
    int choice;
    do {
        cout << "===== Auto Deliver System =====" << endl;
        cout << "1. Add Drone" << endl;
        cout << "2. Edit Drone" << endl;
        cout << "3. Delete Drone" << endl;
        cout << "4. Add Package" << endl;
        cout << "5. Edit Package" << endl;
        cout << "6. Delete Package" << endl;
        cout << "7. Planning options" << endl;
        cout << "8. Save plan" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addDrone(autoDeliver);
            break;
        case 2:
            editDrone(autoDeliver);
            break;
        case 3:
            deleteDrone(autoDeliver);
            break;
        case 4:
            addPackage(autoDeliver);
            break;
        case 5:
            editPackage(autoDeliver);
            break;
        case 6:
            deletePackage(autoDeliver);
            break;
        case 7:
            generateAndDisplayPlan(autoDeliver);
            break;
        case 8:
            savePlan(autoDeliver);
            break;
        case 0:
            cout << "Exiting the Auto Deliver System." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }

        cout << endl;
    } while (choice != 0);
}

bool UserInterface::fileExists(const string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}

bool UserInterface::isPathValid(const string& path) {
    ifstream file(path);
    return file.good();
}

void UserInterface::addDrone(AutoDeliver& autoDeliver) {
    string id, destination;
    int hours, minutes;
    int capacityTypeChoice;
    cout << "Enter Drone ID: ";
    cin >> id;
    cout << "Enter Destination: ";
    cin >> destination;
    cout << "Enter Time (hours minutes): ";
    cin >> hours >> minutes;
    cout << "Choose Capacity Type (1: Mini, 2: Maxi, 3: Heavy): ";
    cin >> capacityTypeChoice;
    CapacityType capacityType;
    switch (capacityTypeChoice) {
    case 1:
        capacityType = CapacityType::MINI;
        break;
    case 2:
        capacityType = CapacityType::MAXI;
        break;
    case 3:
        capacityType = CapacityType::HEAVY;
        break;
    default:
        cout << "Invalid choice. Defaulting to Mini." << endl;
        capacityType = CapacityType::MINI;
        break;
    }
    autoDeliver.addDrone(Drone(id, destination, hours, minutes, capacityType));
}

void UserInterface::editDrone(AutoDeliver& autoDeliver) {
    string id, newId, destination;
    int hours, minutes;
    int capacityTypeChoice;
    cout << "Enter Drone ID to Edit: ";
    cin >> id;
    cout << "Enter New Drone ID: ";
    cin >> newId;
    cout << "Enter New Destination: ";
    cin >> destination;
    cout << "Enter New Time (hours minutes): ";
    cin >> hours >> minutes;
    cout << "Choose New Capacity Type (1: Mini, 2: Maxi, 3: Heavy): ";
    cin >> capacityTypeChoice;
    CapacityType capacityType;
    switch (capacityTypeChoice) {
    case 1:
        capacityType = CapacityType::MINI;
        break;
    case 2:
        capacityType = CapacityType::MAXI;
        break;
    case 3:
        capacityType = CapacityType::HEAVY;
        break;
    default:
        cout << "Invalid choice. Defaulting to Mini." << endl;
        capacityType = CapacityType::MINI;
        break;
    }
    autoDeliver.editDrone(id, Drone(newId, destination, hours, minutes, capacityType));
}

void UserInterface::deleteDrone(AutoDeliver& autoDeliver) {
    string id;
    cout << "Enter Drone ID to Delete: ";
    cin >> id;
    autoDeliver.deleteDrone(id);
}

void UserInterface::addPackage(AutoDeliver& autoDeliver) {
    string id, destination;
    int hours, minutes;
    cout << "Enter Package ID: ";
    cin >> id;
    cout << "Enter Destination: ";
    cin >> destination;
    cout << "Enter Time (hours minutes): ";
    cin >> hours >> minutes;
    autoDeliver.addPackage(Package(id, destination, hours, minutes));
}

void UserInterface::editPackage(AutoDeliver& autoDeliver) {
    string id, newId, destination;
    int hours, minutes;
    cout << "Enter Package ID to Edit: ";
    cin >> id;
    cout << "Enter New Package ID: ";
    cin >> newId;
    cout << "Enter New Destination: ";
    cin >> destination;
    cout << "Enter New Time (hours minutes): ";
    cin >> hours >> minutes;
    autoDeliver.editPackage(id, Package(newId, destination, hours, minutes));
}

void UserInterface::deletePackage(AutoDeliver& autoDeliver) {
    string id;
    cout << "Enter Package ID to Delete: ";
    cin >> id;
    autoDeliver.deletePackage(id);
}

void UserInterface::generateAndDisplayPlan(AutoDeliver& autoDeliver) {
    int planChoice;
    cout << "===== Display and Plan Options =====" << endl;
    cout << "1. By Drone Capacity" << endl;
    cout << "2. Optimised plan" << endl;
    cout << "3. Display NotAtFullLoadCapacity" << endl;
    cout << "4. Display NotAssignedToDrone" << endl;
    cout << "Enter your choice: ";

    if (!(cin >> planChoice)) {
        // The input is not an integer, clear the stream and ignore the invalid input
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid integer choice." << endl;
    }
    else {
        switch (planChoice) {
        case 1:
            autoDeliver.generateMatchingPlan_OptimizeCapacity("capacity_plan.txt");
            break;
        case 2:
            autoDeliver.generateMatchingPlan_MinimiseDrones();
            break;
        case 3:
            autoDeliver.displayDrones_NotAtFullLoadCapacity();
            break;
        case 4:    
            autoDeliver.displayPackages_NotAssignedToDrone();
            break;
        default:
            cout << "Invalid choice for matching plan generation." << endl;
            break;
        }
        
    }
}

void UserInterface::savePlan(AutoDeliver& autoDeliver) {
    autoDeliver.saveMatchingPlan("matching_plan.txt");
    cout << "Matching plan saved to matching_plan.txt." << endl;
}

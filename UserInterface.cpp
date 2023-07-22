#include "UserInterface.h"
#include <iostream>
#include <fstream>

void UserInterface::start() {
    AutoDeliver autoDeliver;

    // Load data from files
    std::string droneFilePath, packageFilePath;

    std::cout << "Enter the file path for drones: ";
    std::cin >> droneFilePath;
    if (!isPathValid(droneFilePath) || !fileExists(droneFilePath)) {
        std::cerr << "Error: Invalid or not found drones.txt file." << std::endl;
        return;
    }

    std::cout << "Enter the file path for packages: ";
    std::cin >> packageFilePath;
    if (!isPathValid(packageFilePath) || !fileExists(packageFilePath)) {
        std::cerr << "Error: Invalid or not found packages.txt file." << std::endl;
        return;
    }

    autoDeliver.readFiles(droneFilePath, packageFilePath);

    // User interface loop
    int choice;
    do {
        std::cout << "===== Auto Deliver System =====" << std::endl;
        std::cout << "1. Add Drone" << std::endl;
        std::cout << "2. Edit Drone" << std::endl;
        std::cout << "3. Delete Drone" << std::endl;
        std::cout << "4. Add Package" << std::endl;
        std::cout << "5. Edit Package" << std::endl;
        std::cout << "6. Delete Package" << std::endl;
        std::cout << "7. Generate and display plan" << std::endl;
        std::cout << "8. Save plan" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

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
            std::cout << "Exiting the Auto Deliver System." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
        }

        std::cout << std::endl;
    } while (choice != 0);
}

bool UserInterface::fileExists(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}

bool UserInterface::isPathValid(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}

void UserInterface::addDrone(AutoDeliver& autoDeliver) {
    std::string id, destination;
    int hours, minutes;
    int capacityTypeChoice;
    std::cout << "Enter Drone ID: ";
    std::cin >> id;
    std::cout << "Enter Destination: ";
    std::cin >> destination;
    std::cout << "Enter Time (hours minutes): ";
    std::cin >> hours >> minutes;
    std::cout << "Choose Capacity Type (1: Mini, 2: Maxi, 3: Heavy): ";
    std::cin >> capacityTypeChoice;
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
        std::cout << "Invalid choice. Defaulting to Mini." << std::endl;
        capacityType = CapacityType::MINI;
        break;
    }
    autoDeliver.addDrone(Drone(id, destination, hours, minutes, capacityType));
}

void UserInterface::editDrone(AutoDeliver& autoDeliver) {
    std::string id, newId, destination;
    int hours, minutes;
    int capacityTypeChoice;
    std::cout << "Enter Drone ID to Edit: ";
    std::cin >> id;
    std::cout << "Enter New Drone ID: ";
    std::cin >> newId;
    std::cout << "Enter New Destination: ";
    std::cin >> destination;
    std::cout << "Enter New Time (hours minutes): ";
    std::cin >> hours >> minutes;
    std::cout << "Choose New Capacity Type (1: Mini, 2: Maxi, 3: Heavy): ";
    std::cin >> capacityTypeChoice;
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
        std::cout << "Invalid choice. Defaulting to Mini." << std::endl;
        capacityType = CapacityType::MINI;
        break;
    }
    autoDeliver.editDrone(id, Drone(newId, destination, hours, minutes, capacityType));
}

void UserInterface::deleteDrone(AutoDeliver& autoDeliver) {
    std::string id;
    std::cout << "Enter Drone ID to Delete: ";
    std::cin >> id;
    autoDeliver.deleteDrone(id);
}

void UserInterface::addPackage(AutoDeliver& autoDeliver) {
    std::string id, destination;
    int hours, minutes;
    std::cout << "Enter Package ID: ";
    std::cin >> id;
    std::cout << "Enter Destination: ";
    std::cin >> destination;
    std::cout << "Enter Time (hours minutes): ";
    std::cin >> hours >> minutes;
    autoDeliver.addPackage(Package(id, destination, hours, minutes));
}

void UserInterface::editPackage(AutoDeliver& autoDeliver) {
    std::string id, newId, destination;
    int hours, minutes;
    std::cout << "Enter Package ID to Edit: ";
    std::cin >> id;
    std::cout << "Enter New Package ID: ";
    std::cin >> newId;
    std::cout << "Enter New Destination: ";
    std::cin >> destination;
    std::cout << "Enter New Time (hours minutes): ";
    std::cin >> hours >> minutes;
    autoDeliver.editPackage(id, Package(newId, destination, hours, minutes));
}

void UserInterface::deletePackage(AutoDeliver& autoDeliver) {
    std::string id;
    std::cout << "Enter Package ID to Delete: ";
    std::cin >> id;
    autoDeliver.deletePackage(id);
}

void UserInterface::generateAndDisplayPlan(AutoDeliver& autoDeliver) {
    int planChoice;
    cout << "Display and Plan Options" << endl;
    cout << "1. By Drone Capacity" << endl;
    cout << "2. Optimised plan" << endl;
    cout << "3. Display Drones and Packages" << endl;
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
            autoDeliver.generateMatchingPlan_OptimizeCapacity();
            break;
        case 2:
            autoDeliver.generateMatchingPlan_MinimiseDrones();
            break;
        case 3:
            autoDeliver.displayDrones_NotAtFullLoadCapacity();
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
    std::cout << "Matching plan saved to matching_plan.txt." << std::endl;
}

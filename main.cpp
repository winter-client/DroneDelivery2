// main.cpp

#include <iostream>
#include <fstream>
#include "AutoDeliver.h"

bool fileExists(const std::string& filePath) {
    ifstream file(filePath);
    return file.good();
}

bool isPathValid(const std::string& path) {
    ifstream file(path);
    return file.good();
}

int main() {
    AutoDeliver autoDeliver;

    // Load data from files
    std::string droneFilePath, packageFilePath;

    cout << "Enter the file path for drones: ";
    cin >> droneFilePath;
    if (!isPathValid(droneFilePath) || !fileExists(droneFilePath)) {
        cerr << "Error: Invalid or not found drones.txt file." << endl;
        return 1;
    }

    cout << "Enter the file path for packages: ";
    cin >> packageFilePath;
    if (!isPathValid(packageFilePath) || !fileExists(packageFilePath)) {
        cerr << "Error: Invalid or not found packages.txt file." << endl;
        return 1;
    }

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
        cout << "7. Generate and display plan" << endl;
        cout << "8. Save plan" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
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
            break;
        }
        case 2: {
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
            break;
        }
        case 3: {
            std::string id;
            std::cout << "Enter Drone ID to Delete: ";
            std::cin >> id;
            autoDeliver.deleteDrone(id);
            break;
        }
        case 4: {
            std::string id, destination;
            int hours, minutes;
            std::cout << "Enter Package ID: ";
            std::cin >> id;
            std::cout << "Enter Destination: ";
            std::cin >> destination;
            std::cout << "Enter Time (hours minutes): ";
            std::cin >> hours >> minutes;
            autoDeliver.addPackage(Package(id, destination, hours, minutes));
            break;
        }
        case 5: {
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
            break;
        }
        case 6: {
            std::string id;
            std::cout << "Enter Package ID to Delete: ";
            std::cin >> id;
            autoDeliver.deletePackage(id);
            break;
        }
        case 7:
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
            break;

        case 8:
            autoDeliver.saveMatchingPlan("matching_plan.txt");
            std::cout << "Matching plan saved to matching_plan.txt." << std::endl;
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

    return 0;
}

#include "../include/services/AuthService.hpp"
#include "../include/services/RideService.hpp"
#include "../include/models/User.hpp"
#include "../include/models/RideDetails.hpp"
#include "../include/utils/DateTime.hpp"
#include <iostream>
#include <limits> // For numeric_limits

// Function to clear input buffer after a bad read
void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Function to get user input for a string
std::string getUserInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin >> std::ws, input);
    return input;
}

// Function to print ride details
void printRideDetails(const Ride& ride) {
    std::cout << "--------------------------------\n";
    std::cout << "Ride ID: " << ride.rideId << "\n";
    std::cout << "From: " << ride.pickupLocation << "\n";
    std::cout << "To: " << ride.dropLocation << "\n";
    std::cout << "Status: ";
    switch (ride.status) {
        case RideStatus::REQUESTED: std::cout << "Requested"; break;
        case RideStatus::ONGOING: std::cout << "Ongoing"; break;
        case RideStatus::COMPLETED: std::cout << "Completed"; break;
        case RideStatus::CANCELLED: std::cout << "Cancelled"; break;
    }
    std::cout << "\n";
    std::cout << "--------------------------------\n";
}

int main() {
    AuthService authService;
    RideService rideService;
    User* loggedInUser = nullptr;

    // Dummy user data for testing
    User dummyRider = {"user_1", "Jane Doe", "jane@email.com", "1234567890", "password123", UserRole::RIDER, "", DateTime::getCurrentTimeMillis()};
    User dummyDriver = {"user_2", "John Smith", "john@email.com", "0987654321", "password456", UserRole::DRIVER, "LIC12345", DateTime::getCurrentTimeMillis()};
    authService.registerUser(dummyRider);
    authService.registerUser(dummyDriver);

    int choice;
    std::string email, password;
    
    // Main CLI loop
    while (loggedInUser == nullptr) {
        std::cout << "\n--- Welcome to RideApp CLI ---\n";
        std::cout << "1. Login\n";
        std::cout << "2. Register\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            clearInputBuffer();
            continue;
        }

        switch (choice) {
            case 1: {
                std::cout << "Enter email: ";
                std::cin >> email;
                std::cout << "Enter password: ";
                std::cin >> password;
                loggedInUser = authService.login(email, password);
                break;
            }
            case 2: {
                User newUser;
                std::string roleStr;
                newUser.userId = "user_" + std::to_string(authService.getUsers().size() + 1);
                newUser.fullName = getUserInput("Enter full name: ");
                newUser.email = getUserInput("Enter email: ");
                newUser.phone = getUserInput("Enter phone number: ");
                newUser.passwordHash = getUserInput("Enter password: ");
                roleStr = getUserInput("Enter role (rider/driver): ");
                
                if (roleStr == "rider") {
                    newUser.role = UserRole::RIDER;
                } else if (roleStr == "driver") {
                    newUser.role = UserRole::DRIVER;
                    newUser.licenseNumber = getUserInput("Enter license number: ");
                } else {
                    std::cout << "Invalid role. Please enter 'rider' or 'driver'.\n";
                    continue;
                }
                newUser.createdAt = DateTime::getCurrentTimeMillis();
                authService.registerUser(newUser);
                break;
            }
            case 3:
                std::cout << "Exiting RideApp. Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    }

    // Main menu after login
    while (loggedInUser != nullptr) {
        std::cout << "\n--- Main Menu ---\n";
        if (loggedInUser->role == UserRole::RIDER) {
            std::cout << "1. Request a ride\n";
            std::cout << "2. Cancel a ride\n";
            std::cout << "3. Make a payment\n";
            std::cout << "4. Rate a ride\n";
            std::cout << "5. View my ride history\n";
        } else { // Driver
            std::cout << "1. Register vehicle\n";
            std::cout << "2. View ride requests\n";
            std::cout << "3. Accept a ride\n";
            std::cout << "4. Start a ride\n";
            std::cout << "5. Complete a ride\n";
            std::cout << "6. View my ride history\n";
        }
        std::cout << "7. Logout\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            clearInputBuffer();
            continue;
        }

        if (loggedInUser->role == UserRole::RIDER) {
            switch (choice) {
                case 1: { // Request a ride
                    std::string pickup = getUserInput("Enter pickup location: ");
                    std::string dropoff = getUserInput("Enter dropoff location: ");
                    rideService.requestRide(loggedInUser->userId, pickup, dropoff);
                    break;
                }
                case 2: { // Cancel a ride
                    std::string rideId = getUserInput("Enter ride ID to cancel: ");
                    rideService.cancelRide(rideId);
                    break;
                }
                case 3: { // Make a payment
                    std::string rideId = getUserInput("Enter ride ID to pay for: ");
                    double amount;
                    std::cout << "Enter amount to pay: ";
                    std::cin >> amount;
                    if (std::cin.fail()) {
                        std::cout << "Invalid amount. Returning to menu.\n";
                        std::cin.clear();
                        clearInputBuffer();
                        break;
                    }
                    std::string method = getUserInput("Enter payment method: ");
                    rideService.makePayment(rideId, amount, method);
                    break;
                }
                case 4: { // Rate a ride
                    std::string rideId = getUserInput("Enter ride ID to rate: ");
                    int score;
                    std::cout << "Enter score (1-5): ";
                    std::cin >> score;
                    if (std::cin.fail() || score < 1 || score > 5) {
                        std::cout << "Invalid score. Returning to menu.\n";
                        std::cin.clear();
                        clearInputBuffer();
                        break;
                    }
                    std::string comment = getUserInput("Enter a comment: ");
                    Ride* ride = rideService.getRideById(rideId);
                    if (ride) {
                        // We need the driver's ID to rate them
                        std::string driverId = ride->driverId;
                        rideService.rateRide(rideId, loggedInUser->userId, driverId, score, comment);
                    } else {
                        std::cout << "Ride not found.\n";
                    }
                    break;
                }
                case 5: { // View history
                    auto history = rideService.getRiderHistory(loggedInUser->userId);
                    if (history.empty()) {
                        std::cout << "No ride history found.\n";
                    } else {
                        std::cout << "\n--- Your Ride History ---\n";
                        for (const auto& ride : history) {
                            printRideDetails(ride);
                        }
                    }
                    break;
                }
                case 7:
                    std::cout << "Logging out...\n";
                    loggedInUser = nullptr;
                    break;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
                    break;
            }
        } else { // Driver
            switch (choice) {
                case 1: { // Register vehicle
                    std::string make = getUserInput("Enter vehicle make: ");
                    std::string model = getUserInput("Enter vehicle model: ");
                    std::string plate = getUserInput("Enter plate number: ");
                    std::string color = getUserInput("Enter color: ");
                    int year;
                    std::cout << "Enter year: ";
                    std::cin >> year;
                    if (std::cin.fail()) {
                        std::cout << "Invalid year. Returning to menu.\n";
                        std::cin.clear();
                        clearInputBuffer();
                        break;
                    }
                    rideService.registerVehicle(loggedInUser->userId, make, model, plate, color, year);
                    break;
                }
                case 2: { // View ride requests
                    auto requests = rideService.getRideRequests();
                    if (requests.empty()) {
                        std::cout << "No pending ride requests.\n";
                    } else {
                        std::cout << "\n--- Pending Ride Requests ---\n";
                        for (const auto& req : requests) {
                            printRideDetails(req);
                        }
                    }
                    break;
                }
                case 3: { // Accept a ride
                    std::string rideId = getUserInput("Enter ride ID to accept: ");
                    rideService.acceptRide(rideId, loggedInUser->userId);
                    break;
                }
                case 4: { // Start a ride
                    std::string rideId = getUserInput("Enter ride ID to start: ");
                    rideService.startRide(rideId);
                    break;
                }
                case 5: { // Complete a ride
                    std::string rideId = getUserInput("Enter ride ID to complete: ");
                    rideService.completeRide(rideId);
                    break;
                }
                case 6: { // View history
                    auto history = rideService.getDriverHistory(loggedInUser->userId);
                    if (history.empty()) {
                        std::cout << "No ride history found.\n";
                    } else {
                        std::cout << "\n--- Your Ride History ---\n";
                        for (const auto& ride : history) {
                            printRideDetails(ride);
                        }
                    }
                    break;
                }
                case 7:
                    std::cout << "Logging out...\n";
                    loggedInUser = nullptr;
                    break;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
                    break;
            }
        }
    }
    return 0;
}
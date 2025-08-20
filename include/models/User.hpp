#pragma once

#include <string>

// Enum for user role
enum class UserRole {
    RIDER,
    DRIVER
};

// Struct to hold user details
struct User {
    std::string userId;
    std::string fullName;
    std::string email;
    std::string phone;
    std::string passwordHash;
    UserRole role;
    std::string licenseNumber; // Only for drivers
    long long createdAt;
};

// Struct to hold vehicle details
struct Vehicle {
    std::string vehicleId;
    std::string driverId;
    std::string make;
    std::string model;
    std::string plateNumber;
    std::string color;
    int year;
};
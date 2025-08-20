#pragma once

#include <string>

// Enum for ride status
enum class RideStatus {
    REQUESTED,
    ONGOING,
    COMPLETED,
    CANCELLED
};

// Enum for payment status
enum class PaymentStatus {
    PENDING,
    COMPLETED,
    FAILED
};

// Struct to hold ride details
struct Ride {
    std::string rideId;
    std::string riderId;
    std::string driverId;
    std::string pickupLocation;
    std::string dropLocation;
    RideStatus status;
    long long startTime;
    long long endTime;
    double fare;
};

// Struct to hold payment details
struct Payment {
    std::string paymentId;
    std::string rideId;
    double amount;
    std::string method;
    PaymentStatus status;
    long long paymentDate;
};

// Struct to hold rating details
struct Rating {
    std::string ratingId;
    std::string rideId;
    std::string givenBy;
    std::string givenTo;
    int score;
    std::string comment;
};
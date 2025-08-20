// --- C:\Users\ChandanaG\OneDrive - IBM\Desktop\RideApp\src\services\RideService.cpp ---
#include "../include/services/RideService.hpp"
#include "../include/utils/DateTime.hpp"
#include "../include/utils/Hashing.hpp"
#include <iostream>
#include <algorithm>

Ride* RideService::requestRide(const std::string& riderId, const std::string& pickupLocation, const std::string& dropLocation) {
    Ride newRide;
    newRide.rideId = "RIDE_" + std::to_string(rides.size() + 1);
    newRide.riderId = riderId;
    newRide.pickupLocation = pickupLocation;
    newRide.dropLocation = dropLocation;
    newRide.status = RideStatus::REQUESTED;
    newRide.startTime = 0;
    newRide.endTime = 0;
    newRide.fare = 0.0;
    rides.push_back(newRide);
    std::cout << "Ride " << newRide.rideId << " requested from " << pickupLocation << " to " << dropLocation << ".\n";
    return &rides.back();
}

void RideService::registerVehicle(const std::string& driverId, const std::string& make, const std::string& model, const std::string& plateNumber, const std::string& color, int year) {
    for (const auto& vehicle : vehicles) {
        if (vehicle.plateNumber == plateNumber) {
            std::cout << "Error: Vehicle with this plate number already exists.\n";
            return;
        }
    }
    Vehicle newVehicle;
    newVehicle.vehicleId = "VEHICLE_" + std::to_string(vehicles.size() + 1);
    newVehicle.driverId = driverId;
    newVehicle.make = make;
    newVehicle.model = model;
    newVehicle.plateNumber = plateNumber;
    newVehicle.color = color;
    newVehicle.year = year;
    vehicles.push_back(newVehicle);
    std::cout << "Vehicle " << newVehicle.plateNumber << " registered successfully.\n";
}

std::vector<Ride> RideService::getRideRequests() const {
    std::vector<Ride> requests;
    for (const auto& ride : rides) {
        if (ride.status == RideStatus::REQUESTED) {
            requests.push_back(ride);
        }
    }
    return requests;
}

bool RideService::acceptRide(const std::string& rideId, const std::string& driverId) {
    Ride* ride = getRideById(rideId);
    if (ride && ride->status == RideStatus::REQUESTED) {
        ride->status = RideStatus::ONGOING;
        ride->driverId = driverId;
        std::cout << "Ride " << rideId << " accepted by driver " << driverId << ".\n";
        return true;
    }
    std::cout << "Error: Ride could not be accepted.\n";
    return false;
}

bool RideService::startRide(const std::string& rideId) {
    Ride* ride = getRideById(rideId);
    if (ride && ride->status == RideStatus::ONGOING) {
        ride->startTime = DateTime::getCurrentTimeMillis();
        std::cout << "Ride " << rideId << " has started.\n";
        return true;
    }
    std::cout << "Error: Ride could not be started.\n";
    return false;
}

bool RideService::completeRide(const std::string& rideId) {
    Ride* ride = getRideById(rideId);
    if (ride && ride->status == RideStatus::ONGOING) {
        ride->endTime = DateTime::getCurrentTimeMillis();
        ride->status = RideStatus::COMPLETED;
        // Calculate a dummy fare
        ride->fare = 15.0 + (rand() % 20); // Random fare for demo
        std::cout << "Ride " << rideId << " completed. Fare is $" << ride->fare << ".\n";
        return true;
    }
    std::cout << "Error: Ride could not be completed.\n";
    return false;
}

bool RideService::cancelRide(const std::string& rideId) {
    Ride* ride = getRideById(rideId);
    if (ride && (ride->status == RideStatus::REQUESTED || ride->status == RideStatus::ONGOING)) {
        ride->status = RideStatus::CANCELLED;
        std::cout << "Ride " << rideId << " has been cancelled.\n";
        return true;
    }
    std::cout << "Error: Ride could not be cancelled.\n";
    return false;
}

bool RideService::makePayment(const std::string& rideId, double amount, const std::string& method) {
    Ride* ride = getRideById(rideId);
    if (ride && ride->status == RideStatus::COMPLETED) {
        Payment newPayment;
        newPayment.paymentId = "PAYMENT_" + std::to_string(payments.size() + 1);
        newPayment.rideId = rideId;
        newPayment.amount = amount;
        newPayment.method = method;
        newPayment.status = PaymentStatus::COMPLETED;
        newPayment.paymentDate = DateTime::getCurrentTimeMillis();
        payments.push_back(newPayment);
        std::cout << "Payment of $" << amount << " for ride " << rideId << " completed.\n";
        return true;
    }
    std::cout << "Error: Cannot make payment for this ride.\n";
    return false;
}

bool RideService::rateRide(const std::string& rideId, const std::string& givenBy, const std::string& givenTo, int score, const std::string& comment) {
    Ride* ride = getRideById(rideId);
    if (ride && ride->status == RideStatus::COMPLETED) {
        Rating newRating;
        newRating.ratingId = "RATING_" + std::to_string(ratings.size() + 1);
        newRating.rideId = rideId;
        newRating.givenBy = givenBy;
        newRating.givenTo = givenTo;
        newRating.score = score;
        newRating.comment = comment;
        ratings.push_back(newRating);
        std::cout << "Rating for " << givenTo << " submitted successfully.\n";
        return true;
    }
    std::cout << "Error: Cannot rate this ride.\n";
    return false;
}

Ride* RideService::getRideById(const std::string& rideId) {
    for (size_t i = 0; i < rides.size(); ++i) {
        if (rides[i].rideId == rideId) {
            return &rides[i];
        }
    }
    return nullptr;
}

std::vector<Ride> RideService::getRiderHistory(const std::string& riderId) const {
    std::vector<Ride> history;
    for (const auto& ride : rides) {
        if (ride.riderId == riderId) {
            history.push_back(ride);
        }
    }
    return history;
}

std::vector<Ride> RideService::getDriverHistory(const std::string& driverId) const {
    std::vector<Ride> history;
    for (const auto& ride : rides) {
        if (ride.driverId == driverId) {
            history.push_back(ride);
        }
    }
    return history;
}
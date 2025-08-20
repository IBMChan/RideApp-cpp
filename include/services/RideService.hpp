#pragma once

#include "../models/RideDetails.hpp"
#include "include/models/User.hpp"
#include <vector>
#include <string>

class RideService {
public:
    // Rider
    Ride* requestRide(const std::string& riderId, const std::string& pickupLocation, const std::string& dropLocation);
    bool cancelRide(const std::string& rideId);
    bool makePayment(const std::string& rideId, double amount, const std::string& method);
    bool rateRide(const std::string& rideId, const std::string& givenBy, const std::string& givenTo, int score, const std::string& comment);

    // Driver
    void registerVehicle(const std::string& driverId, const std::string& make, const std::string& model, const std::string& plateNumber, const std::string& color, int year);
    std::vector<Ride> getRideRequests() const;
    bool acceptRide(const std::string& rideId, const std::string& driverId);
    bool startRide(const std::string& rideId);
    bool completeRide(const std::string& rideId);

    // Shared
    Ride* getRideById(const std::string& rideId);
    std::vector<Ride> getRiderHistory(const std::string& riderId) const;
    std::vector<Ride> getDriverHistory(const std::string& driverId) const;

private:
    // In-memory storage for rides, vehicles, payments, and ratings
    std::vector<Ride> rides;
    std::vector<Vehicle> vehicles;
    std::vector<Payment> payments;
    std::vector<Rating> ratings;
};
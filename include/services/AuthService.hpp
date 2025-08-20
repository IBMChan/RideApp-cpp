#pragma once

#include "include/models/User.hpp"
#include <string>
#include <vector>

class AuthService {
public:
    // User registration
    bool registerUser(const User& user);
    
    // User login
    User* login(const std::string& email, const std::string& password);
    
    // Check if email already exists
    bool isEmailRegistered(const std::string& email);

    // Get all users
    const std::vector<User>& getUsers() const;

private:
    std::string hashPassword(const std::string& password);
    // In-memory user storage
    std::vector<User> users;
};
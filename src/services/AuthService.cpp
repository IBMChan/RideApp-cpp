#include "../include/services/AuthService.hpp"
#include "../include/utils/Hashing.hpp"
#include "../include/utils/DateTime.hpp"
#include <iostream>

bool AuthService::registerUser(const User& newUser) {
    if (isEmailRegistered(newUser.email)) {
        std::cout << "Error: Email already registered.\n";
        return false;
    }
    
    // Hash the password before storing
    User user = newUser;
    user.passwordHash = hashPassword(newUser.passwordHash);
    users.push_back(user);
    std::cout << "Registration successful for user " << user.fullName << ".\n";
    return true;
}

User* AuthService::login(const std::string& email, const std::string& password) {
    std::string hashedPassword = hashPassword(password);
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].email == email && users[i].passwordHash == hashedPassword) {
            std::cout << "Login successful! Welcome, " << users[i].fullName << ".\n";
            return &users[i];
        }
    }
    std::cout << "Login failed: Invalid credentials.\n";
    return nullptr;
}

bool AuthService::isEmailRegistered(const std::string& email) {
    for (const auto& user : users) {
        if (user.email == email) {
            return true;
        }
    }
    return false;
}

const std::vector<User>& AuthService::getUsers() const {
    return users;
}

std::string AuthService::hashPassword(const std::string& password) {
    // A simple, non-secure hash for demonstration
    return Hashing::hash(password);
}


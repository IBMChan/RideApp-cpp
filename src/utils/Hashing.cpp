#include "../include/utils/Hashing.hpp"
#include <functional> // For std::hash

std::string Hashing::hash(const std::string& input) {
    // A simple, non-secure hash function for demonstration purposes
    return std::to_string(std::hash<std::string>{}(input));
}
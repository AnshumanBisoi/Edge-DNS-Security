#include "Logger.h"
#include <fstream>
#include <iostream>
#include <ctime>

Logger::Logger(const std::string& filename)
    : filename(filename) {
}

void Logger::log(const std::string& domain,
                 const std::string& action) {

    std::ofstream file(filename, std::ios::app);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open log file."
                  << std::endl;
        return;
    }

    std::time_t currentTime = std::time(nullptr);

    file << std::ctime(&currentTime)
         << "Domain: " << domain
         << " | Action: " << action
         << std::endl;

    file.close();
}


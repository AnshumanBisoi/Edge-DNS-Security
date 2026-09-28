#include "DNSFilter.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>

DNSFilter::DNSFilter() {}

void DNSFilter::loadBlockedDomains(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open blocklist: "
                  << filename << std::endl;
        return;
    }

    std::string domain;

    while (std::getline(file, domain)) {
        if (!domain.empty()) {
            blockedDomains.insert(domain);
        }
    }

    file.close();

    std::cout << "Loaded "
              << blockedDomains.size()
              << " blocked domains." << std::endl;
}

bool DNSFilter::isBlocked(const std::string& domain) const {
    return blockedDomains.find(domain) != blockedDomains.end();
}


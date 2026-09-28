#include "DNSResolver.h"
#include <iostream>

DNSResolver::DNSResolver() {}

bool DNSResolver::resolve(const std::string& domain) {
    std::cout << "[DNSResolver] Resolving: " << domain << std::endl;

    // Basic resolver simulation
    if (domain.empty()) {
        return false;
    }

    std::cout << "[DNSResolver] Query accepted." << std::endl;
    return true;
}

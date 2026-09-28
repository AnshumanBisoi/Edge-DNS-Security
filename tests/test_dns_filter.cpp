#include <iostream>
#include "../src/DNSFilter.h"

int main() {
    DNSFilter filter;

    filter.loadBlockedDomains("config/blocked_domains.txt");

    std::cout << "\n=== DNS Filter Test ===\n";

    std::string domains[] = {
        "google.com",
        "malicious.test",
        "phishing.test",
        "example.com"
    };

    for (const std::string& domain : domains) {
        if (filter.isBlocked(domain)) {
            std::cout << domain << " -> BLOCKED" << std::endl;
        } else {
            std::cout << domain << " -> ALLOWED" << std::endl;
        }
    }

    return 0;
}

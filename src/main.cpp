#include <iostream>
#include <string>

#include "DNSFilter.h"
#include "DNSResolver.h"
#include "Logger.h"

int main() {
    DNSFilter filter;
    DNSResolver resolver;
    Logger logger("logs/dns_activity.log");

    filter.loadBlockedDomains("config/blocked_domains.txt");

    std::string domain;

    std::cout << "\n=== Edge DNS Security Gateway ===\n";
    std::cout << "Enter a domain to check (type 'exit' to quit).\n\n";

    while (true) {
        std::cout << "Domain: ";
        std::cin >> domain;

        if (domain == "exit") {
            break;
        }

        if (filter.isBlocked(domain)) {
            std::cout << "BLOCKED: " << domain << std::endl;
            logger.log(domain, "BLOCKED");
        } else {
            std::cout << "ALLOWED: " << domain << std::endl;

            if (resolver.resolve(domain)) {
                logger.log(domain, "ALLOWED");
            } else {
                logger.log(domain, "RESOLUTION_FAILED");
            }
        }

        std::cout << std::endl;
    }

    std::cout << "DNS Security Gateway stopped.\n";

    return 0;
}


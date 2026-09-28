
#include <iostream>
#include <string>

#include "DNSFilter.h"
#include "DNSResolver.h"
#include "DNSCache.h"
#include "Logger.h"
#include "Statistics.h"

int main() {

    DNSFilter filter;
    DNSResolver resolver;
    DNSCache cache(10);
    Logger logger("logs/dns_activity.log");
    Statistics stats;

    // Load blocked domains
    filter.loadBlockedDomains("config/blocked_domains.txt");

    std::string domain;

    std::cout << "\n=== Edge DNS Security Gateway ===\n";
    std::cout << "Enter a domain to check (type 'exit' to quit).\n\n";

    while (true) {

        std::cout << "Domain: ";
        std::cin >> domain;

        // Exit
        if (domain == "exit") {
            break;
        }

        // Every entered domain is a query
        stats.recordAllowed();

        // --------------------------------
        // 1. Check blocked domain
        // --------------------------------
        if (filter.isBlocked(domain)) {

            std::cout << "BLOCKED: " << domain << std::endl;

            stats.recordBlocked();
            logger.log(domain, "BLOCKED");

            continue;
        }

        // --------------------------------
        // 2. Check DNS Cache
        // --------------------------------
        std::string cachedResponse;

        if (cache.get(domain, cachedResponse)) {

            std::cout << "CACHE HIT: "
                      << domain
                      << " -> "
                      << cachedResponse
                      << std::endl;

            stats.recordCacheHit();
            logger.log(domain, "CACHE_HIT");

            continue;
        }

        // --------------------------------
        // 3. Resolve DNS
        // --------------------------------
        bool resolved = resolver.resolve(domain);

        if (resolved) {

            std::cout << "ALLOWED: " << domain << std::endl;

            // Store result in cache
            cache.put(domain, "RESOLVED");

            logger.log(domain, "ALLOWED");

        } else {

            std::cout << "DNS RESOLUTION FAILED: "
                      << domain
                      << std::endl;

            logger.log(domain, "RESOLUTION_FAILED");
        }
    }

    // --------------------------------
    // Display statistics
    // --------------------------------
    stats.print();

    std::cout << "\nDNS Security Gateway stopped.\n";

    return 0;
}


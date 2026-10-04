#include <iostream>
#include <string>

#include "DNSFilter.h"
#include "DNSResolver.h"
#include "DNSCache.h"
#include "DNSPacket.h"
#include "DNSAnomalyDetector.h"
#include "Logger.h"
#include "Statistics.h"

int main() {

    DNSFilter filter;
    DNSResolver resolver;

    DNSCache cache(10);

    DNSAnomalyDetector anomalyDetector;

    Logger logger(
        "logs/dns_activity.log"
    );

    Statistics stats;

    // --------------------------------
    // Load blocked domains
    // --------------------------------

    filter.loadBlockedDomains(
        "config/blocked_domains.txt"
    );

    std::string domain;

    std::cout
        << "\n=== Edge DNS Sinkhole & Protocol Anomaly Interceptor ===\n";

    std::cout
        << "Enter a domain to check "
        << "(type 'exit' to quit).\n\n";

    while (true) {

        std::cout << "Domain: ";

        std::cin >> domain;

        // --------------------------------
        // Exit
        // --------------------------------

        if (domain == "exit") {
            break;
        }

        // Every entered domain is a query

        stats.recordAllowed();

        // --------------------------------
        // 1. Protocol Anomaly Detection
        // --------------------------------

        if (
            anomalyDetector.isAnomalous(domain)
        ) {

            std::string reason =
                anomalyDetector.getReason(domain);

            std::cout
                << "PROTOCOL ANOMALY DETECTED: "
                << domain
                << std::endl;

            std::cout
                << "Reason: "
                << reason
                << std::endl;

            std::cout
                << "INTERCEPTED"
                << std::endl;

            logger.log(
                domain,
                "ANOMALY_INTERCEPTED:" + reason
            );

            continue;
        }

        // --------------------------------
        // 2. DNS Sinkhole / Blocklist
        // --------------------------------

        if (
            filter.isBlocked(domain)
        ) {

            std::cout
                << "SINKHOLE: "
                << domain
                << " -> BLOCKED"
                << std::endl;

            stats.recordBlocked();

            logger.log(
                domain,
                "SINKHOLE_BLOCKED"
            );

            continue;
        }

        // --------------------------------
        // 3. DNS Cache
        // --------------------------------

        std::string cachedResponse;

        if (
            cache.get(
                domain,
                cachedResponse
            )
        ) {

            std::cout
                << "CACHE HIT: "
                << domain
                << " -> "
                << cachedResponse
                << std::endl;

            stats.recordCacheHit();

            logger.log(
                domain,
                "CACHE_HIT"
            );

            continue;
        }

        // --------------------------------
        // 4. Real UDP DNS Resolution
        // --------------------------------

        std::string resolvedIP;

        bool resolved =
            resolver.resolve(
                domain,
                resolvedIP
            );

        if (resolved) {

            std::cout
                << "ALLOWED: "
                << domain
                << " -> "
                << resolvedIP
                << std::endl;

            // Store actual IP in cache

            cache.put(
                domain,
                resolvedIP
            );

            logger.log(
                domain,
                "ALLOWED:" + resolvedIP
            );

        } else {

            std::cout
                << "DNS RESOLUTION FAILED: "
                << domain
                << std::endl;

            logger.log(
                domain,
                "RESOLUTION_FAILED"
            );
        }
    }

    // --------------------------------
    // Display statistics
    // --------------------------------

    stats.print();

    std::cout
        << "\nDNS Security Gateway stopped."
        << std::endl;

    return 0;
}


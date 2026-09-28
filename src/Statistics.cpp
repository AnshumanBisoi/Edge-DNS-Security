#include "Statistics.h"
#include <iostream>

Statistics::Statistics()
    : totalQueries(0),
      allowedQueries(0),
      blockedQueries(0),
      cacheHits(0) {}

void Statistics::recordAllowed() {
    totalQueries++;
    allowedQueries++;
}

void Statistics::recordBlocked() {
    totalQueries++;
    blockedQueries++;
}

void Statistics::recordCacheHit() {
    cacheHits++;
}

void Statistics::print() const {
    std::cout << "\n=== DNS Statistics ===\n";
    std::cout << "Total Queries : " << totalQueries << "\n";
    std::cout << "Allowed       : " << allowedQueries << "\n";
    std::cout << "Blocked       : " << blockedQueries << "\n";
    std::cout << "Cache Hits    : " << cacheHits << "\n";
}


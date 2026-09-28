#include "../src/DNSCache.h"
#include <iostream>

int main() {
    DNSCache cache(60);

    cache.put("google.com", "142.250.183.14");

    std::string response;

    if (cache.get("google.com", response)) {
        std::cout << "CACHE HIT: google.com -> "
                  << response << std::endl;
    } else {
        std::cout << "CACHE MISS: google.com" << std::endl;
    }

    if (cache.get("example.com", response)) {
        std::cout << "CACHE HIT: example.com -> "
                  << response << std::endl;
    } else {
        std::cout << "CACHE MISS: example.com" << std::endl;
    }

    std::cout << "Cache entries: "
              << cache.size() << std::endl;

    return 0;
}

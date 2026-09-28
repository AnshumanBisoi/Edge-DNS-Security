#include <iostream>
#include "../src/DNSResolver.h"

int main() {
    DNSResolver resolver;

    std::cout << "=== DNS Resolver Test ===" << std::endl;

    if (resolver.resolve("google.com")) {
        std::cout << "google.com -> RESOLVED" << std::endl;
    } else {
        std::cout << "google.com -> FAILED" << std::endl;
    }

    if (resolver.resolve("example.com")) {
        std::cout << "example.com -> RESOLVED" << std::endl;
    } else {
        std::cout << "example.com -> FAILED" << std::endl;
    }

    return 0;
}


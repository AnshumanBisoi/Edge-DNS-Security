#include "DNSResolver.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>

DNSResolver::DNSResolver() {}

bool DNSResolver::resolve(const std::string& domain) {
    std::cout << "[DNSResolver] Resolving: "
              << domain << std::endl;

    struct addrinfo hints {};
    struct addrinfo* result = nullptr;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(
        domain.c_str(),
        nullptr,
        &hints,
        &result
    );

    if (status != 0) {
        std::cerr << "[DNSResolver] Resolution failed: "
                  << gai_strerror(status)
                  << std::endl;

        return false;
    }

    char ipAddress[INET_ADDRSTRLEN];

    struct sockaddr_in* address =
        reinterpret_cast<struct sockaddr_in*>(result->ai_addr);

    inet_ntop(
        AF_INET,
        &(address->sin_addr),
        ipAddress,
        INET_ADDRSTRLEN
    );

    std::cout << "[DNSResolver] IP Address: "
              << ipAddress << std::endl;

    freeaddrinfo(result);

    return true;
}

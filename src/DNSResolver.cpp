#include "DNSResolver.h"
#include "DNSPacket.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <vector>

DNSResolver::DNSResolver() {}

bool DNSResolver::resolve(
    const std::string& domain,
    std::string& ipAddress
) {

    std::cout << "[DNSResolver] Resolving: "
              << domain << std::endl;

    // --------------------------------
    // 1. Build DNS query packet
    // --------------------------------

    DNSPacket dnsPacket;
    dnsPacket.setQuery(domain);

    std::vector<uint8_t> query =
        dnsPacket.buildQuery();

    // --------------------------------
    // 2. Create UDP socket
    // --------------------------------

    int sock = socket(
        AF_INET,
        SOCK_DGRAM,
        0
    );

    if (sock < 0) {

        std::cerr
            << "[DNSResolver] Socket creation failed: "
            << std::strerror(errno)
            << std::endl;

        return false;
    }

    // --------------------------------
    // 3. Set timeout
    // --------------------------------

    timeval timeout{};

    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    setsockopt(
        sock,
        SOL_SOCKET,
        SO_RCVTIMEO,
        &timeout,
        sizeof(timeout)
    );

    // --------------------------------
    // 4. Configure DNS server
    // --------------------------------

    sockaddr_in server{};

    server.sin_family = AF_INET;
    server.sin_port = htons(53);

    if (
        inet_pton(
            AF_INET,
            "8.8.8.8",
            &server.sin_addr
        ) != 1
    ) {

        std::cerr
            << "[DNSResolver] Invalid DNS server address."
            << std::endl;

        close(sock);

        return false;
    }

    // --------------------------------
    // 5. Send DNS query
    // --------------------------------

    ssize_t sent = sendto(
        sock,
        query.data(),
        query.size(),
        0,
        reinterpret_cast<sockaddr*>(&server),
        sizeof(server)
    );

    if (sent < 0) {

        std::cerr
            << "[DNSResolver] Failed to send DNS packet: "
            << std::strerror(errno)
            << std::endl;

        close(sock);

        return false;
    }

    std::cout
        << "[DNSResolver] UDP DNS query sent."
        << std::endl;

    // --------------------------------
    // 6. Receive DNS response
    // --------------------------------

    uint8_t buffer[512];

    sockaddr_in response{};

    socklen_t responseLength =
        sizeof(response);

    ssize_t received = recvfrom(
        sock,
        buffer,
        sizeof(buffer),
        0,
        reinterpret_cast<sockaddr*>(&response),
        &responseLength
    );

    if (received < 0) {

        std::cerr
            << "[DNSResolver] DNS response failed: "
            << std::strerror(errno)
            << std::endl;

        close(sock);

        return false;
    }

    std::cout
        << "[DNSResolver] DNS response received: "
        << received
        << " bytes"
        << std::endl;

    close(sock);

    // --------------------------------
    // 7. Validate DNS response size
    // --------------------------------

    if (received < 12) {

        std::cerr
            << "[DNSResolver] Invalid DNS response."
            << std::endl;

        return false;
    }

    // --------------------------------
    // 8. Check transaction ID
    // --------------------------------

    uint16_t responseId =
        (static_cast<uint16_t>(buffer[0]) << 8) |
        static_cast<uint16_t>(buffer[1]);

    if (responseId != 0x1234) {

        std::cerr
            << "[DNSResolver] Transaction ID mismatch."
            << std::endl;

        return false;
    }

    // --------------------------------
    // 9. Read DNS flags
    // --------------------------------

    uint16_t flags =
        (static_cast<uint16_t>(buffer[2]) << 8) |
        static_cast<uint16_t>(buffer[3]);

    // Check response flag

    if ((flags & 0x8000) == 0) {

        std::cerr
            << "[DNSResolver] Packet is not a DNS response."
            << std::endl;

        return false;
    }

    // --------------------------------
    // 10. Check DNS response code
    // --------------------------------

    uint16_t rcode =
        flags & 0x000F;

    if (rcode != 0) {

        std::cerr
            << "[DNSResolver] DNS server returned error code: "
            << rcode
            << std::endl;

        return false;
    }

    // --------------------------------
    // 11. Number of answers
    // --------------------------------

    uint16_t answerCount =
        (static_cast<uint16_t>(buffer[6]) << 8) |
        static_cast<uint16_t>(buffer[7]);

    std::cout
        << "[DNSResolver] Answer records: "
        << answerCount
        << std::endl;

    // --------------------------------
    // 12. Find IPv4 address
    // --------------------------------

    for (
        ssize_t i = 12;
        i + 13 < received;
        ++i
    ) {

        uint16_t type =
            (static_cast<uint16_t>(buffer[i]) << 8) |
            static_cast<uint16_t>(buffer[i + 1]);

        uint16_t dnsClass =
            (static_cast<uint16_t>(buffer[i + 2]) << 8) |
            static_cast<uint16_t>(buffer[i + 3]);

        uint16_t dataLength =
            (static_cast<uint16_t>(buffer[i + 8]) << 8) |
            static_cast<uint16_t>(buffer[i + 9]);

        // A record + IN class + 4-byte IPv4 address

        if (
            type == 1 &&
            dnsClass == 1 &&
            dataLength == 4 &&
            i + 13 < received
        ) {

            char resolvedIP[
                INET_ADDRSTRLEN
            ];

            inet_ntop(
                AF_INET,
                &buffer[i + 10],
                resolvedIP,
                INET_ADDRSTRLEN
            );

            ipAddress = resolvedIP;

            std::cout
                << "[DNSResolver] IP Address: "
                << ipAddress
                << std::endl;

            return true;
        }
    }

    // --------------------------------
    // No IPv4 address found
    // --------------------------------

    std::cerr
        << "[DNSResolver] No IPv4 address found."
        << std::endl;

    return false;
}

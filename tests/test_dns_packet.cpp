#include <iostream>
#include "../src/DNSPacket.h"

int main() {
    DNSPacket packet;

    packet.setQuery("google.com");

    std::vector<uint8_t> query = packet.buildQuery();

    std::cout << "DNS Query for: "
              << packet.getDomain() << std::endl;

    std::cout << "Packet size: "
              << query.size() << " bytes" << std::endl;

    std::cout << "Packet data: ";

    for (uint8_t byte : query) {
        printf("%02X ", byte);
    }

    std::cout << std::endl;

    return 0;
}

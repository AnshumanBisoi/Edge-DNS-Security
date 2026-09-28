#include "DNSPacket.h"
#include <cstdint>

DNSPacket::DNSPacket() {
    header.id = 0x1234;
    header.flags = 0x0100;
    header.qdCount = 1;
    header.anCount = 0;
    header.nsCount = 0;
    header.arCount = 0;
}

void DNSPacket::setQuery(const std::string& domain) {
    this->domain = domain;
}

std::string DNSPacket::getDomain() const {
    return domain;
}

void DNSPacket::encodeDomain(std::vector<uint8_t>& packet) {
    size_t start = 0;

    while (start < domain.length()) {
        size_t end = domain.find('.', start);

        if (end == std::string::npos) {
            end = domain.length();
        }

        size_t length = end - start;
        packet.push_back(static_cast<uint8_t>(length));

        for (size_t i = start; i < end; ++i) {
            packet.push_back(static_cast<uint8_t>(domain[i]));
        }

        start = end + 1;
    }

    packet.push_back(0);
}

std::vector<uint8_t> DNSPacket::buildQuery() {
    std::vector<uint8_t> packet;

    packet.push_back((header.id >> 8) & 0xFF);
    packet.push_back(header.id & 0xFF);

    packet.push_back((header.flags >> 8) & 0xFF);
    packet.push_back(header.flags & 0xFF);

    packet.push_back((header.qdCount >> 8) & 0xFF);
    packet.push_back(header.qdCount & 0xFF);

    packet.push_back(0);
    packet.push_back(0);
    packet.push_back(0);
    packet.push_back(0);
    packet.push_back(0);
    packet.push_back(0);

    encodeDomain(packet);

    // QTYPE = A
    packet.push_back(0);
    packet.push_back(1);

    // QCLASS = IN
    packet.push_back(0);
    packet.push_back(1);

    return packet;
}

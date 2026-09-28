#ifndef DNS_PACKET_H
#define DNS_PACKET_H

#include <cstdint>
#include <string>
#include <vector>

struct DNSHeader {
    uint16_t id;
    uint16_t flags;
    uint16_t qdCount;
    uint16_t anCount;
    uint16_t nsCount;
    uint16_t arCount;
};

class DNSPacket {
public:
    DNSPacket();

    void setQuery(const std::string& domain);

    std::vector<uint8_t> buildQuery();

    std::string getDomain() const;

private:
    DNSHeader header;
    std::string domain;

    void encodeDomain(std::vector<uint8_t>& packet);
};

#endif

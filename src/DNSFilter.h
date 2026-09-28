#ifndef DNS_FILTER_H
#define DNS_FILTER_H

#include <string>
#include <unordered_set>

class DNSFilter {
private:
    std::unordered_set<std::string> blockedDomains;

public:
    DNSFilter();

    void loadBlockedDomains(const std::string& filename);
    bool isBlocked(const std::string& domain) const;
};

#endif


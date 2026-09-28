#ifndef DNS_RESOLVER_H
#define DNS_RESOLVER_H

#include <string>

class DNSResolver {
public:
    DNSResolver();

    bool resolve(const std::string& domain);
};

#endif


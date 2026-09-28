#ifndef DNSCACHE_H
#define DNSCACHE_H

#include <string>
#include <unordered_map>
#include <chrono>

class DNSCache {
private:
    struct CacheEntry {
        std::string response;
        std::chrono::steady_clock::time_point expiry;
    };

    std::unordered_map<std::string, CacheEntry> cache;
    int ttlSeconds;

public:
    DNSCache(int ttl = 60);

    void put(const std::string& domain, const std::string& response);

    bool get(const std::string& domain, std::string& response);

    void clear();

    size_t size() const;
};

#endif

#include "DNSCache.h"

DNSCache::DNSCache(int ttl)
    : ttlSeconds(ttl) {}

void DNSCache::put(const std::string& domain, const std::string& response) {
    CacheEntry entry;
    entry.response = response;
    entry.expiry = std::chrono::steady_clock::now()
                  + std::chrono::seconds(ttlSeconds);

    cache[domain] = entry;
}

bool DNSCache::get(const std::string& domain, std::string& response) {
    auto it = cache.find(domain);

    if (it == cache.end()) {
        return false;
    }

    if (std::chrono::steady_clock::now() >= it->second.expiry) {
        cache.erase(it);
        return false;
    }

    response = it->second.response;
    return true;
}

void DNSCache::clear() {
    cache.clear();
}

size_t DNSCache::size() const {
    return cache.size();
}

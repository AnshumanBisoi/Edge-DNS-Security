# Edge DNS Security Gateway

A C++17 based DNS security gateway that filters DNS requests at the edge.

## Features

- DNS domain resolution
- Blocklist-based domain filtering
- DNS response caching
- TTL-based cache expiration
- DNS activity logging
- Query statistics
- Cache-hit detection

## Technologies

- C++17
- Linux
- DNS
- Git
- UTM Ubuntu Virtual Machine

## Security Features

The gateway blocks configured malicious domains such as:

- malicious.test
- phishing.test
- malware.test

Allowed domains are resolved normally.

## Cache

DNS responses are stored temporarily in an in-memory cache.

When the same domain is requested again before the TTL expires:

CACHE HIT

After the TTL expires, the domain is resolved again.

## Statistics

The gateway records:

- Total queries
- Allowed queries
- Blocked queries
- Cache hits

## Example

google.com → RESOLVED

google.com → CACHE HIT

malicious.test → BLOCKED

phishing.test → BLOCKED

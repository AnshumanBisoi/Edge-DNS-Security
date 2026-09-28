# Product Requirements Document

## 1. Project Name

Edge DNS Security Gateway

## 2. Purpose

The Edge DNS Security Gateway is a C++17 based system designed to inspect and control DNS requests at the edge.

The system checks requested domains against a local blocklist, resolves allowed domains, caches DNS results, records activity, and maintains query statistics.

## 3. Objectives

- Filter blocked domains.
- Resolve allowed DNS requests.
- Reduce repeated DNS lookups using caching.
- Record DNS security events.
- Maintain query statistics.
- Provide a modular C++ implementation.
- Demonstrate Linux and system programming concepts.

## 4. Functional Requirements

### FR1 — Domain Filtering

The system shall check every requested domain against the configured blocklist.

### FR2 — DNS Resolution

The system shall resolve domains that are not blocked.

### FR3 — DNS Cache

The system shall store resolved results in an in-memory cache.

### FR4 — TTL Expiration

Cached entries shall expire after the configured TTL.

### FR5 — Logging

The system shall record DNS activities in a log file.

### FR6 — Statistics

The system shall maintain:

- Total queries
- Allowed queries
- Blocked queries
- Cache hits

### FR7 — User Interface

The system shall provide a command-line interface where users can enter domains and receive the security decision.

## 5. Non-Functional Requirements

- Written in C++17.
- Runs on Linux.
- Modular source-code structure.
- Configuration stored separately from source code.
- Testable individual components.
- Git-based version control.

## 6. Security Requirements

The gateway shall:

- Block configured malicious domains.
- Prevent blocked domains from being resolved.
- Record blocked requests.
- Provide visibility through activity logs.

## 7. Expected Output

Example:

```text
Domain: google.com
ALLOWED: google.com

Domain: google.com
CACHE HIT: google.com -> RESOLVED

Domain: malicious.test
BLOCKED: malicious.test

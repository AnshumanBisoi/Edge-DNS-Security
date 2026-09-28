# Edge DNS Security Gateway

## 1. Project Overview

Edge DNS Security Gateway is a C++17 based DNS security and filtering system designed to inspect DNS requests before allowing them to proceed.

The system provides domain filtering, DNS resolution, DNS response caching, activity logging, and query statistics.

The project is developed and tested on Linux using Ubuntu running inside a UTM virtual machine.

---

## 2. Objectives

The main objectives are:

- Implement a DNS security gateway using C++.
- Filter configured blocked domains.
- Resolve allowed domains using the system DNS resolver.
- Cache DNS results temporarily using TTL.
- Record DNS security activity.
- Maintain query statistics.
- Demonstrate Linux system programming concepts.
- Use Git for version control and project development.

---

## 3. Main Features

### DNS Filtering

Domains listed in:

`config/blocked_domains.txt`

are blocked before DNS resolution.

Example:

```text
malicious.test -> BLOCKED
phishing.test  -> BLOCKED


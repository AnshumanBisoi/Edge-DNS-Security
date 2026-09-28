cmake_minimum_required(VERSION 3.16)

project(EdgeDNSSecurity LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

# Main DNS Security Gateway
add_executable(dns_security
    src/main.cpp
    src/DNSFilter.cpp
    src/DNSResolver.cpp
    src/DNSCache.cpp
    src/DNSPacket.cpp
    src/DNSAnomalyDetector.cpp
    src/Logger.cpp
    src/Statistics.cpp
)

# DNS Protocol Anomaly Test
add_executable(test_dns_anomaly
    tests/test_dns_anomaly.cpp
    src/DNSAnomalyDetector.cpp
)

target_include_directories(test_dns_anomaly PRIVATE src)

# Enable CTest
enable_testing()

# Register the test
add_test(
    NAME DNSAnomalyTest
    COMMAND test_dns_anomaly
)


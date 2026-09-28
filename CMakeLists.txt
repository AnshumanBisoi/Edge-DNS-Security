cmake_minimum_required(VERSION 3.16)

project(EdgeDNSSecurity LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

add_executable(dns_security
    src/main.cpp
    src/DNSFilter.cpp
    src/DNSResolver.cpp
    src/DNSCache.cpp
    src/DNSPacket.cpp
    src/Logger.cpp
    src/Statistics.cpp
)

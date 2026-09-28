#include "DNSAnomalyDetector.h"

#include <cctype>
#include <sstream>
#include <vector>

bool DNSAnomalyDetector::hasInvalidCharacters(
    const std::string& domain) const {

    for (char c : domain) {

        if (std::isalnum(static_cast<unsigned char>(c)) ||
            c == '-' ||
            c == '.') {
            continue;
        }

        return true;
    }

    return false;
}

bool DNSAnomalyDetector::hasLongLabel(
    const std::string& domain) const {

    std::stringstream ss(domain);
    std::string label;

    while (std::getline(ss, label, '.')) {

        if (label.empty()) {
            return true;
        }

        if (label.length() > 63) {
            return true;
        }
    }

    return false;
}

bool DNSAnomalyDetector::isAnomalous(
    const std::string& domain) const {

    if (domain.empty()) {
        return true;
    }

    if (domain.length() > 253) {
        return true;
    }

    if (hasInvalidCharacters(domain)) {
        return true;
    }

    if (hasLongLabel(domain)) {
        return true;
    }

    return false;
}

std::string DNSAnomalyDetector::getReason(
    const std::string& domain) const {

    if (domain.empty()) {
        return "EMPTY_DOMAIN";
    }

    if (domain.length() > 253) {
        return "DOMAIN_TOO_LONG";
    }

    if (hasInvalidCharacters(domain)) {
        return "INVALID_CHARACTERS";
    }

    if (hasLongLabel(domain)) {
        return "INVALID_LABEL";
    }

    return "NORMAL";
}


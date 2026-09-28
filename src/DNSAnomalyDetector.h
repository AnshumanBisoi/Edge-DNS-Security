#ifndef DNS_ANOMALY_DETECTOR_H
#define DNS_ANOMALY_DETECTOR_H

#include <string>

class DNSAnomalyDetector {
public:
    bool isAnomalous(const std::string& domain) const;
    std::string getReason(const std::string& domain) const;

private:
    bool hasInvalidCharacters(const std::string& domain) const;
    bool hasLongLabel(const std::string& domain) const;
};

#endif


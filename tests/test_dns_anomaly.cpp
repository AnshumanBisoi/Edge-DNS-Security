
#include <iostream>
#include "DNSAnomalyDetector.h"

int main() {

    DNSAnomalyDetector detector;

    std::cout << "=== DNS Protocol Anomaly Test ===\n";

    std::string normal = "google.com";
    std::string invalidCharacter = "google@.com";
    std::string emptyLabel = "google..com";

    std::cout << normal << " -> "
              << (detector.isAnomalous(normal) ? "ANOMALY" : "NORMAL")
              << std::endl;

    std::cout << invalidCharacter << " -> "
              << (detector.isAnomalous(invalidCharacter)
                      ? "ANOMALY"
                      : "NORMAL")
              << std::endl;

    std::cout << "Reason: "
              << detector.getReason(invalidCharacter)
              << std::endl;

    std::cout << emptyLabel << " -> "
              << (detector.isAnomalous(emptyLabel)
                      ? "ANOMALY"
                      : "NORMAL")
              << std::endl;

    std::cout << "Reason: "
              << detector.getReason(emptyLabel)
              << std::endl;

    return 0;
}

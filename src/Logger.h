#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    Logger(const std::string& filename);

    void log(const std::string& domain, const std::string& action);

private:
    std::string filename;
};

#endif


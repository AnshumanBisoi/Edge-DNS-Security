#ifndef STATISTICS_H
#define STATISTICS_H

class Statistics {
private:
    int totalQueries;
    int allowedQueries;
    int blockedQueries;
    int cacheHits;

public:
    Statistics();

    void recordAllowed();
    void recordBlocked();
    void recordCacheHit();

    void print() const;
};

#endif


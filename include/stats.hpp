#ifndef STATS_HPP
#define STATS_HPP
#include <mutex>
#include <string>

class Stats {
private:
    static int successCount;
    static int failureCount;
    static int missedCount;

    static std::mutex statsMutex;

 public:
    static void addSuccess();
    static void addFailure();
    static void addMissed();

    static int getSuccess();
    static int getFailure();
    static int getMissed();

    static int calculateScore();
    static void printStats();
    static void saveHighScore(const std::string& playerName);
    static void printLeaderboard();
    static void updateRecords(const std::string& playerName);
    static void printRecords();
};

#endif
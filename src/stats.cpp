#include "stats.hpp"
#include "color.hpp"
#include <iostream>
#include <fstream>
using namespace std;

// initializing static members to prove that I know how to do it!
int Stats::successCount = 0;
int Stats::failureCount = 0;
int Stats::missedCount = 0;
std::mutex Stats::statsMutex;

void Stats::addSuccess() {
    std::lock_guard<std::mutex> lock(statsMutex);
    successCount++;
}

void Stats::addFailure() {
    std::lock_guard<std::mutex> lock(statsMutex);
    failureCount++;
}

void Stats::addMissed() {
    std::lock_guard<std::mutex> lock(statsMutex);
    missedCount++;
}

int Stats::getSuccess() {
    std::lock_guard<std::mutex> lock(statsMutex);
    return successCount;
}

int Stats::getFailure() {
    std::lock_guard<std::mutex> lock(statsMutex);
    return failureCount;
}

int Stats::getMissed() {
    std::lock_guard<std::mutex> lock(statsMutex);
    return missedCount;
}

// Scoring system(simpler to understand). Might be changed or truncated if need be!!!!
int Stats::calculateScore() {
    //std::lock_guard<std::mutex> lock(statsMutex);
    return successCount * 20 - failureCount * 3 - missedCount * 5;
}

/*void Stats::printStats() {
    std::lock_guard<std::mutex> lock(statsMutex);
    cout << "\n=== GAME STATS ===\n";
    cout << "Successful Dispatches: " << successCount << endl;
    cout << "Hero Failures: " << failureCount << endl;
    cout << "Unattended Calls: " << missedCount << endl;
    cout << "Total Score: " << calculateScore() << endl;
}*/
void Stats::printStats() {
    std::lock_guard<std::mutex> lock(statsMutex);

    int currentScore = calculateScore();
    int previousHigh = INT32_MIN;

    // Check previous high score
    ifstream inFile("highscore.txt");
    if (inFile.is_open()) {
        inFile >> previousHigh;
        inFile.close();
    }

    cout << "\n" << Color::magenta("=== GAME STATS ===") << "\n";
    cout << Color::magenta("Successful Dispatches: ") << successCount << endl;
    cout << Color::magenta("Hero Failures: ") << failureCount << endl;
    cout << Color::magenta("Unattended Calls: ") << missedCount << endl;
    cout << Color::magenta("Total Score: ") << currentScore << endl;

    if (currentScore > previousHigh) {
        cout << Color::bold(Color::yellow("\n>>>AYOO! NEW HIGH SCORE!! <<<")) << endl;
    }
}

// saving the highest score as of now, might delete this feature later!!
void Stats::saveHighestScore() {
    std::lock_guard<std::mutex> lock(statsMutex);
    int currentScore = calculateScore();
    int previousHigh = INT32_MIN;

    ifstream inFile("highscore.txt");
    if (inFile.is_open()) {
        inFile >> previousHigh;
        inFile.close();
    }

    if (currentScore > previousHigh) {
        ofstream outFile("highscore.txt");
        if (outFile.is_open()) {
            outFile << currentScore;
            outFile.close();
        }
    }
}
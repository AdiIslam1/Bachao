#include "stats.hpp"
#include "color.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <mutex>

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

    /*if (currentScore > previousHigh) {
        cout << Color::bold(Color::yellow("\n>>>AYOO! NEW HIGH SCORE!! <<<")) << endl;
    }*/
}

// saving the highest scores as of now, might delete this feature later!!
void Stats::saveHighScore(const string &playerName) {
    lock_guard<mutex> lock(statsMutex);

    int currentScore = calculateScore();

    // Step 1: Initialize leaderboard with 10 placeholders
    vector<pair<string, int>> leaderboard(10, {"Waiting", -1000});

    // Step 2: Overwrite with actual file contents if available
    ifstream inFile("highscore.txt");
    string line;
    int i = 0;
    while (getline(inFile, line) && i < 10) {
        string name; int score;
        stringstream ss(line);
        ss >> name >> score;
        leaderboard[i] = {name, score};
        i++;
    }
    inFile.close();

    // Step 3: Insert current score into leaderboard
    bool Top10 = false;
    int pos;
    for (int i = 0; i < 10; i++) {
        if (currentScore > leaderboard[i].second) {
            Top10 = true;
            pos=i+1;
            leaderboard.insert(leaderboard.begin() + i, {playerName, currentScore});
            break;
        }
    }

    if (leaderboard.size() > 10)
        leaderboard.resize(10);

    // Step 4: Update the file
    ofstream outFile("highscore.txt");
    for (int i = 0; i < 10; i++)
        outFile << leaderboard[i].first << " " << leaderboard[i].second << "\n";
    outFile.close();

    if (Top10){
        if(pos==1) cout << Color::bold(Color::yellow("\n>>> REJOICE! You have conquered the SYSTEM <<<\n"));
        else if(pos<=3) cout << Color::bold(Color::red("\n>>> IMPRESSIVE! You got on the podium. Have what it takes to reach the top? <<<\n"));
        else cout << Color::bold(Color::cyan("\n>>> CONGRATS! You are in the TOP 10. Bet you can't rise any higher. <<<\n"));
    }
}

void Stats::printLeaderboard() {
    lock_guard<mutex> lock(statsMutex);

    // Step 1: Initialize leaderboard with placeholders
    vector<pair<string, int>> leaderboard(10, {"Waiting", -1000});

    // Step 2: Overwrite with file contents
    ifstream inFile("highscore.txt");
    string line;
    int i = 0;
    while (getline(inFile, line) && i < 10) {
        string name; int score;
        stringstream ss(line);
        ss >> name >> score;
        leaderboard[i] = {name, score};
        i++;
    }
    inFile.close();

    // Step 3: Print neatly
    cout << "\n" << Color::magenta("=== TOP 10 LEADERBOARD ===") << "\n\n";
    cout << left << setw(5) << "Rank" 
         << setw(15) << "Player" 
         << right << setw(7) << "Score" << "\n";
    cout << string(27, '-') << "\n";

    for (int i = 0; i < 10; i++) {
        cout << left << setw(5) << (i + 1)
             << setw(15) << leaderboard[i].first
             << right << setw(7) << leaderboard[i].second << "\n";
    }
    cout << endl;
}





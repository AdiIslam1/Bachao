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

    // Initialize leaderboard with 10 players (randomly choosing Waiting for no reason)
    vector<pair<string, int>> leaderboard(10, {"Waiting", -1000});

    // Overwriting with actual file contents if available
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

    // Inserting current score into leaderboard
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

    // Updating the file
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
    //Initializing
    vector<pair<string, int>> leaderboard(10, {"Waiting", -1000});

    // Overwriting
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

    // Printing
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

void Stats::updateRecords(const string &playerName) {
    lock_guard<mutex> lock(statsMutex);

    int currentSuccess = successCount;
    double currentSuccessRate = ((double)successCount*100)/(successCount + failureCount + missedCount);

    // Default values
    string bestDispatcherName = "Waiting";
    int bestDispatcherScore = -1000;

    string bestHandlerName = "Waiting";
    int bestHandlerScore = 0;

    // Reading existing records
    ifstream inFile("records.txt");
    if (inFile.is_open()) {
        string label;

        inFile >> label >> bestDispatcherName >> bestDispatcherScore;
        inFile >> label >> bestHandlerName >> bestHandlerScore;

        inFile.close();
    }

    bool newDispatcher = false;
    bool newHandler = false;

    // Updating Best Dispatcher
    if (currentSuccess > bestDispatcherScore) {
        bestDispatcherScore = currentSuccess;
        bestDispatcherName = playerName;
        newDispatcher = true;
    }

    // Updating Best Call Handler (Best call handler implies, the one with the best success rate)
    if (currentSuccessRate > bestHandlerScore) {
        bestHandlerScore = currentSuccessRate;
        bestHandlerName = playerName;
        newHandler = true;
    }

    // Write back to file
    ofstream outFile("records.txt");
    outFile << "BestDispatcher " << bestDispatcherName << " " << bestDispatcherScore << "\n";
    outFile << "BestCallHandler " << bestHandlerName << " " << bestHandlerScore << "\n";
    outFile.close();

    // Messages
    if (newDispatcher) {
        cout << Color::bold(Color::yellow("\n>>> NEW BEST DISPATCHER! <<<\n"));
    }

    if (newHandler) {
        cout << Color::bold(Color::cyan("\n>>> NEW BEST CALL HANDLER! <<<\n"));
    }
}

void Stats::printRecords() {
    lock_guard<mutex> lock(statsMutex);

    ifstream inFile("records.txt");

    if (!inFile.is_open()) {
        cout << "\nNo records yet!\n";
        return;
    }

    string label, name1, name2;
    int score1, score2;

    inFile >> label >> name1 >> score1;
    inFile >> label >> name2 >> score2;

    inFile.close();

    cout << "\n" << Color::magenta("=== ALL-TIME RECORDS ===") << "\n\n";

    cout << Color::yellow("Best Dispatcher: ")
         << name1 << " (" << score1 << ")" << endl;

    cout << Color::cyan("Best Call Handler: ")
         << name2 << " (" << score2 << "%)" << endl;
}


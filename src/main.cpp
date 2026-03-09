#include "console_utils.hpp"
#include <chrono>
#include <color.hpp>
#include <dashboard.hpp>
#include <heroes.hpp>
#include <iostream>
#include <stressCalls.hpp>
#include "stats.hpp"
#include <thread>
#include <limits>
using namespace std;

#include "color.hpp"
#include <iostream>
#include <limits> // Required for clearing bad inputs

using namespace std;

string playerName;
int showMenu() {
    int choice;

    while (true) {
        cout << "\033[2J\033[H"; // Clear screen

        // Epic ASCII Art Title
        cout << Color::red(Color::bold(R"(
  ____    _    ____ _   _    _    ___  
 | __ )  / \  / ___| | | |  / \  / _ \ 
 |  _ \ / _ \| |   | |_| | / _ \| | | |
 | |_) / ___ \ |___|  _  |/ ___ \ |_| |
 |____/_/   \_\____|_| |_/_/   \_\___/ 
        )")) << "\n";

        cout << Color::cyan("======================================") << endl;
        cout << Color::yellow(Color::bold("      STRESS CALLS DISPATCH TERMINAL  ")) << endl;
        cout << Color::cyan("======================================") << endl;

        cout << "\n" << Color::green("[1] ") << Color::bold("Deploy Units") << " (Play Game)\n";
        cout << Color::blue("[2] ") << "Field Manual (Rules)\n";
        cout << Color::magenta("[3] ") << "Team Roster (Credits)\n";
        cout << Color::red("[4] ") << "Exit Terminal\n";

        cout << "\n" << Color::cyan("Enter command parameter (1-4): ");
        
        // Robust input handling! Prevents infinite loops if they type a letter
        if (!(cin >> choice)) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Throw away the bad input
            continue;
        }

        if (choice >= 1 && choice <= 4) {
            return choice;
        }
    }
}

void showRules() {
    cout << "\033[2J\033[H";

    cout << Color::magenta("======================================") << endl;
    cout << Color::yellow(Color::bold("        BACHAO - FIELD MANUAL         ")) << endl;
    cout << Color::magenta("======================================") << endl;

    cout << "\n" << Color::bold(Color::red("THE MISSION:")) << "\n";
    cout << "You have " << Color::yellow("120 seconds") << " to manage the city's emergencies.\n";
    cout << "If a stress call timer hits zero, you suffer a casualty!\n";

    cout << "\n" << Color::bold(Color::blue("DISPATCH PROTOCOLS (POLYMORPHISM):")) << "\n";
    cout << Color::red(" [+] MEDIC") << "       -> Handles " << Color::red("Medical") << " emergencies.\n";
    cout << Color::blue(" [*] POLICE") << "      -> Handles " << Color::blue("Crime") << " & " << Color::magenta("Political") << " unrest.\n";
    cout << Color::yellow(" [!] FIREFIGHTER") << " -> Handles " << Color::yellow("Hazard") << " & " << Color::cyan("Disaster") << " events.\n";

    cout << "\n" << Color::bold(Color::green("RESOURCE MANAGEMENT (STAMINA):")) << "\n";
    cout << "Dispatching a hero costs " << Color::yellow("25 Stamina") << ".\n";
    cout << "If a hero drops below 25 Stamina, they become exhausted and require a longer rest period before their next deployment.\n";

    cout << "\n" << Color::cyan("Press [ENTER] to return to command center...");
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear any leftover newlines
    cin.get(); // Wait for enter
}

void showCredits() {
    cout << "\033[2J\033[H";

    cout << Color::cyan("======================================") << endl;
    cout << Color::yellow(Color::bold("         TEAM OTC - ROSTER            ")) << endl;
    cout << Color::cyan("======================================") << endl;

    cout << "\n" << Color::bold("LEAD ARCHITECTS & DEVELOPERS:\n\n");
    
    // Giving everyone a bit of flair
    cout << " " << Color::red(">>") << " Ahnaf Irtiza Nibir\n";
    cout << " " << Color::green(">>") << " Saad Al Abeed\n";
    cout << " " << Color::blue(">>") << " Adi Islam\n";
    cout << " " << Color::magenta(">>") << " Anas Ibn Aziz\n";

    cout << "\n" << Color::cyan("Press [ENTER] to return to command center...");
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int main() {
  {
    std::lock_guard<std::mutex> lock(ConsoleUtils::consoleMutex);
    cout << "\033[2J\033[H";
    cout << Color::cyan("======================================") << endl;
    cout << Color::yellow(Color::bold(" BACHAO Stress Calls Dispatch Terminal"))
         << endl;
    cout << Color::cyan("======================================") << endl;
  }

  int timeToPlay = 120;
  atomic<bool> isRunning(true);

  StressCall::generate(get_random(2, 4));


  while (true) {
        int choice = showMenu();

        if (choice == 1) {
            cout << "\033[2J\033[H";
            cout << Color::bold("Enter Player Name: ");
            cin >> playerName;
            break; // start game
        }
        else if (choice == 2) {
            showRules();
        }
        else if (choice == 3) {
            showCredits();
        }
        else {
            return 0;
        }
  }
  
  thread worker(StressCall::printCallList, ref(isRunning));
  thread heroWorker(Hero::printHeroList, ref(isRunning));
  thread dashboardWorker(Dashboard::start, ref(isRunning));
  while (timeToPlay > 0) {
    // Every 20 seconds, generate more calls
    if (timeToPlay < 120 && timeToPlay % 20 == 0) {
      StressCall::generate(get_random(1, 3));
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    timeToPlay--;
  }

  // Safe Exit
  while (!StressCall::isEmpty()) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  isRunning = false;
  if (worker.joinable()) {
    worker.join();
  }
  if (heroWorker.joinable()) {
    heroWorker.join();
  }
  if (dashboardWorker.joinable()) {
    dashboardWorker.join();
  }
  {
    std::lock_guard<std::mutex> lock(ConsoleUtils::consoleMutex);
    cout << "\033[2J\033[H"; // Clear screen at end
    Stats::printStats();
    Stats::saveHighScore(playerName);
    Stats::printLeaderboard();
    cout << "\n" << Color::bold("Thanks for playing") << endl;
  } // Player statistics should be shown here
}

#include "console_utils.hpp"
#include <chrono>
#include <color.hpp>
#include <dashboard.hpp>
#include <heroes.hpp>
#include <iostream>
#include <stressCalls.hpp>
#include <thread>
using namespace std;

int showMenu() {
    int choice;

    while (true) {
        cout << "\033[2J\033[H"; // clear screen

        cout << Color::cyan("======================================") << endl;
        cout << Color::yellow(Color::bold(" BACHAO - Main Menu")) << endl;
        cout << Color::cyan("======================================") << endl;

        cout << "\n1. Play Game\n";
        cout << "2. Rules\n";
        cout << "3. Credits\n";
        cout << "4. Exit\n";

        cout << "\nEnter choice: ";
        cin >> choice;

        if (choice >= 1 && choice <= 4)
            return choice;
    }
}

void showRules() {
    cout << "\033[2J\033[H";

    cout << Color::bold("Game Rules\n\n");

    cout << "1. Stress calls will appear randomly.\n";
    cout << "2. Assign the correct hero to solve the problem.\n";
    cout << "3. Medic handles medical emergencies.\n";
    cout << "4. Firefighter handles fire incidents.\n";
    cout << "5. Hero handles crime situations.\n";

    cout << "\nPress enter to return...";
    cin.ignore();
    cin.get();
}

void showCredits() {
    cout << "\033[2J\033[H";

    cout << Color::bold("Credits\n\n");

    cout << "Developed by:\n";
    cout << "Ahnaf Irtiza Nibir\n";
    cout << "Saad Al Abeed\n";
    cout << "Adi Islam\n";
    cout << "Anas Ibn Aziz\n";

    cout << "\nPress enter to return...";
    cin.ignore();
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
    cout << "\n" << Color::bold("Thanks for playing") << endl;
  } // Player statistics should be shown here
}

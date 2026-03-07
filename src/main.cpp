#include "console_utils.hpp"
#include <chrono>
#include <color.hpp>
#include <dashboard.hpp>
#include <heroes.hpp>
#include <iostream>
#include <stressCalls.hpp>
#include "stats.hpp"
#include <thread>
using namespace std;

int main() {
  {
    std::lock_guard<std::mutex> lock(ConsoleUtils::consoleMutex);
    cout << "\033[2J\033[H";
    cout << Color::cyan("======================================") << endl;
    cout << Color::yellow(Color::bold(" BACHAO Stress Calls Dispatch Terminal"))
         << endl;
    cout << Color::cyan("======================================") << endl;
  }

  int timeToPlay = 20;
  atomic<bool> isRunning(true);

  StressCall::generate(get_random(2, 4));

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
    Stats::saveHighestScore();
    cout << "\n" << Color::bold("Thanks for playing") << endl;
  } // Player statistics should be shown here
}

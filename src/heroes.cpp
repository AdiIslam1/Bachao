#include "heroes.hpp"
#include <sstream>
#include "stats.hpp"

Hero::Hero(const string &heroType)
    : heroType(heroType), name(""), skillLevel(0), status("Available") {}

void Hero::printHeroList(atomic<bool> &isRunning) {
  const int startCol = 100; // Start column for hero list (right side)

  while (isRunning) {
    stringstream ss;
    // Build buffer without lock (heroList is static and effectively
    // read-only/thread-safe for reads) If strict safety needed, we could add a
    // header mutex, but for now assuming static init is done.

    ss << "\0337"; // Save cursor position
    int row = 1;
    auto moveCursorSS = [&](int r, int c) {
      ss << "\033[" << r << ";" << c << "H";
    };

    moveCursorSS(row++, startCol);
    ss << Color::cyan("======================================");
    moveCursorSS(row++, startCol);
    ss << Color::yellow(Color::bold(" BACHAO Hero Roster"));
    moveCursorSS(row++, startCol);
    ss << Color::cyan("======================================");
    moveCursorSS(row++, startCol);
    ss << "=== AVAILABLE HEROES ===";
    moveCursorSS(row++, startCol);
    ss << "Total Heroes: " << heroList.size();
    moveCursorSS(row++, startCol);
    ss << "--------------------------------";

    int idx = 1;
    for (const auto &hero : heroList) {
      moveCursorSS(row++, startCol);
      ss << idx++ << ". ";

      string type = hero.getHeroType();
      if (type == "Medic")
        ss << Color::red("[MEDIC]");
      else if (type == "Police")
        ss << Color::blue("[POLICE]");
      else if (type == "Firefighter")
        ss << Color::yellow("[FIREFIGHTER]");

      ss << " " << hero.getName();

      moveCursorSS(row++, startCol);
      ss << "   Skill Level: " << hero.getSkillLevel();

      string status = hero.getStatus();
      if (status == "Available")
        ss << " | Status: " << Color::green(status);
      else if (status == "On-Duty")
        ss << " | Status: " << Color::yellow(status);
      else if (status == "Resting")
        ss << " | Status: " << Color::cyan(status);
      else if (status == "Dead")
        ss << " | Status: " << Color::red(status);
      else
        ss << " | Status: " << status;

      moveCursorSS(row++, startCol);
      ss << "--------------------------------";
    }
    ss << "\0338"; // Restore cursor position

    if (!ss.str().empty()) {
      std::lock_guard<std::mutex> lock(ConsoleUtils::consoleMutex);
      if (cout.fail())
        cout.clear();
      cout << ss.str();
      cout.flush();
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

// Initialize the heroList with 9 heroes (3 Police, 3 Medics, 3 Firefighters)
vector<Hero> Hero::heroList = {
    // Police
    Police("Officer John", 3, "Available"),
    Police("Officer Sarah", 4, "Available"),
    Police("Officer Mike", 5, "Available"),
    // Medics
    Medic("Dr. Emily", 3, "Available"), Medic("Dr. James", 4, "Available"),
    Medic("Dr. Lisa", 5, "Available"),
    // Firefighters
    Firefighter("Chief Tom", 3, "Available"),
    Firefighter("Captain Alex", 4, "Available"),
    Firefighter("Lt. Rachel", 5, "Available")};

// Write polymorphism functions for the heroes from here

#include "heroes.hpp"
#include <sstream>

Hero::Hero(const string &heroType)
    : heroType(heroType), name(""), skillLevel(0), status("Available"), stamina(100), maxStamina(100) {}

Hero* Hero::getHeroByIndex(int index) {
  index--;
  if (index < 0 || index >= heroList.size())
    return nullptr;
  return heroList[index];
}

void Hero::setSkillLevel(int newSkillLevel) {
    skillLevel = newSkillLevel;
}

void Hero::increaseSkillLevel() {
  int currentSkill = getSkillLevel();
  setSkillLevel(currentSkill + 1);  
}

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

      string type = hero->getHeroType();
      if (type == "Medic")
        ss << Color::red("[MEDIC]");
      else if (type == "Police")
        ss << Color::blue("[POLICE]");
      else if (type == "Firefighter")
        ss << Color::yellow("[FIREFIGHTER]");

      ss << " " << hero->getName();

      moveCursorSS(row++, startCol);
      ss << "   Skill Level: " << hero->getSkillLevel();

      string status = hero->getStatus();
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

      // stamina bar
      moveCursorSS(row++, startCol);
      int barLength = 10;
      int filled = (hero->getStamina() * barLength) / hero->getMaxStamina();
      
      ss << "   Stamina: [";
      for (int i = 0; i < barLength; i++) {
          if (i < filled) {
              // Color it Red if stamina is dangerously low (< 30), otherwise Green
              if (hero->getStamina() < 30) ss << Color::red("|");
              else ss << Color::green("|");
          } else {
              ss << " "; // Empty space for missing stamina
          }
      }
      ss << "] " << hero->getStamina() << "/" << hero->getMaxStamina();
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
vector<Hero*> Hero::heroList = {
    // Police
    new Police("Officer John", 3, "Available"),
    new Police("Officer Sarah", 4, "Available"),
    new Police("Officer Mike", 5, "Available"),
    // Medics
    new Medic("Dr. Emily", 3, "Available"),
    new Medic("Dr. James", 4, "Available"),
    new Medic("Dr. Lisa", 5, "Available"),
    // Firefighters
    new Firefighter("Chief Tom", 3, "Available"),
    new Firefighter("Captain Alex", 4, "Available"),
    new Firefighter("Lt. Rachel", 5, "Available")
};

// Write polymorphism functions for the heroes from here

bool Medic::canHandle(const string& callType) const {
  return callType == "Medical";
}
int Medic::getResolutionTime() const {
  return 30 - skillLevel*2; // Higher skill level means faster resolution
}

bool Police::canHandle(const string& callType) const {
  return (callType == "Crime" || callType == "Political");
}
int Police::getResolutionTime() const {
  return 40 - skillLevel*3; // Higher skill level means faster resolution
}

bool Firefighter::canHandle(const string& callType) const {
  return (callType == "Fire" || callType == "Hazard" || callType == "Disaster");
}
int Firefighter::getResolutionTime() const {
  return 50 - skillLevel*4; // Higher skill level means faster resolution
} 

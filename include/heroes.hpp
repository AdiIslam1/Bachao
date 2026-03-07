#ifndef HEROES_HPP
#define HEROES_HPP

#include "console_utils.hpp"
#include <atomic>
#include <chrono>
#include <color.hpp>
#include <ctime>
#include <iostream>
#include <mutex>
#include <random>
#include <stressCalls.hpp>
#include <string>
#include <thread>
#include <vector>

using namespace std;

// Define polymorphic functions for the heroes accordingly

class Hero {
private:
  static vector<Hero> heroList;

protected:
  string heroType;
  string name;
  int skillLevel;
  string status; // "Available", "On-Duty", "Resting", "Dead"

public:
  Hero(const string &heroType);
  virtual ~Hero() = default;

  // Getters
  string getHeroType() const { return heroType; }
  string getName() const { return name; }
  int getSkillLevel() const { return skillLevel; }
  string getStatus() const { return status; }

  static void printHeroList(atomic<bool> &isRunning);
};

class Medic : public Hero {
public:
  Medic(const string &name, int skillLevel, const string &status)
      : Hero("Medic") {
    this->name = name;
    this->skillLevel = skillLevel;
    this->status = status;
  };
};

class Police : public Hero {
public:
  Police(const string &name, int skillLevel, const string &status)
      : Hero("Police") {
    this->name = name;
    this->skillLevel = skillLevel;
    this->status = status;
  };
};

class Firefighter : public Hero {
public:
  Firefighter(const string &name, int skillLevel, const string &status)
      : Hero("Firefighter") {
    this->name = name;
    this->skillLevel = skillLevel;
    this->status = status;
  };
};

#endif

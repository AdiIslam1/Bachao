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

// Custom Exception Class
class HeroException : public exception {
private:
    string message;
public:
    HeroException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Define polymorphic functions for the heroes accordingly

class Hero {
private:
  static vector<Hero*> heroList;

protected:
  string heroType;
  string name;
  int skillLevel;
  int stamina;
  int maxStamina;
  string status; // "Available", "On-Duty", "Resting", "Dead"

public:
  Hero(const string &heroType);
  virtual ~Hero() = default;

  // Getters
  string getHeroType() const { return heroType; }
  string getName() const { return name; }
  int getSkillLevel() const { return skillLevel; }
  string getStatus() const { return status; }
  int getStamina() const { return stamina; }
  int getMaxStamina() const { return maxStamina; }
  // setters
  void setStatus(const string &newStatus) { status = newStatus; }
  void setSkillLevel(int newSkillLevel);
  void setStamina(int newStamina) { stamina = newStamina; }
  void setMaxStamina(int newMaxStamina) { maxStamina = newMaxStamina; }
  void increaseStamina(int amount) { stamina = min(stamina + amount, maxStamina); }
  void decreaseStamina(int amount) { stamina = max(stamina - amount, 0); }
  // static functions
  static Hero* getHeroByIndex(int index);
  // pure virtual function to be implemented by derived classes
  virtual bool canHandle(const string &newStatus) const = 0;
  virtual int getResolutionTime() const = 0;
  void increaseSkillLevel();
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
  bool canHandle(const string& callType) const override;
  int getResolutionTime() const override;
};

class Police : public Hero {
public:
  Police(const string &name, int skillLevel, const string &status)
      : Hero("Police") {
    this->name = name;
    this->skillLevel = skillLevel;
    this->status = status;
  };
  bool canHandle(const string& callType) const override;
  int getResolutionTime() const override;
};

class Firefighter : public Hero {
public:
  Firefighter(const string &name, int skillLevel, const string &status)
      : Hero("Firefighter") {
    this->name = name;
    this->skillLevel = skillLevel;
    this->status = status;
  };
  bool canHandle(const string& callType) const override;
  int getResolutionTime() const override;
};

#endif

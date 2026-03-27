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

// Custom Exception Classes for the Dashboard Dispatch Logic
class DispatchException : public exception {
protected:
    string errorMessage;
public:
    DispatchException(const string& msg) : errorMessage(msg) {}
    const char* what() const noexcept override {
        return errorMessage.c_str(); 
    }
};

class InvalidHeroException : public DispatchException {
public:
    InvalidHeroException(int heroId) 
        : DispatchException("ERROR: Hero ID " + to_string(heroId) + " does not exist!") {}
};

class InvalidCallException : public DispatchException {
public:
    InvalidCallException(int callId) 
        : DispatchException("ERROR: Call ID " + to_string(callId) + " is invalid or just expired!") {}
};

class HeroUnavailableException : public DispatchException {
public:
    HeroUnavailableException(const string& heroName, const string& status)
        : DispatchException("ERROR: " + heroName + " is currently " + status + "!") {}
};

class IncompatibleHeroException : public DispatchException {
public:
    IncompatibleHeroException(const string& heroType, const string& callType)
        : DispatchException("ERROR: " + heroType + " cannot handle a " + callType + " emergency!") {}
};

class HeroExhaustedException : public DispatchException {
public:
    HeroExhaustedException(const string& heroName) 
        : DispatchException("ERROR: " + heroName + " is resting! Please choose another hero.") {}
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
  void setStatus(const string &newStatus);
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
  Medic(const string &name, int skillLevel, const string &status);
  bool canHandle(const string& callType) const override;
  int getResolutionTime() const override;
};

class Police : public Hero {
public:
  Police(const string &name, int skillLevel, const string &status);
  bool canHandle(const string& callType) const override;
  int getResolutionTime() const override;
};

class Firefighter : public Hero {
public:
  Firefighter(const string &name, int skillLevel, const string &status);
  bool canHandle(const string& callType) const override;
  int getResolutionTime() const override;
};

#endif

#ifndef STRESSCALLS_HPP
#define STRESSCALLS_HPP
using namespace std;
#include "console_utils.hpp"
#include <atomic>
#include <chrono>
#include <color.hpp>
#include <ctime>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <vector>

class StressCall {
private:
  string type; // Medical, Hazard, Crime, Political, Disaster
  string activity;
  string address;
  string priority; // Low, Medium, High, Very high, Critical
  int timeLeft;

public:
  // getters
  string getType() const { return this->type; }
  string getActivity() const { return this->activity; }
  string getAddress() const { return this->address; }
  string getPriority() const { return this->priority; }
  int getTimeLeft() const { return this->timeLeft; }
  static string getCallType(int index);
  static bool resolveCall(int index);
  // setters
  void setType(string type) { this->type = type; }
  void setActivity(string activity) { this->activity = activity; }
  void setAddress(string address) { this->address = address; }
  void setPriority(string priority) { this->priority = priority; }
  // constructor
  StressCall(string type, string activity, string address, string priority,
             int timeLeft) {
    setType(type);
    setActivity(activity);
    setAddress(address);
    setPriority(priority);
    this->timeLeft = timeLeft;
  }

  void tick() {
    if (timeLeft > 0)
      timeLeft--;
  }

  static void tickAll(); // Helper to tick all calls
  static void generate(int level);
  static void printCallList(atomic<bool> &isRunning);
  static bool isEmpty(); // Thread-safe check

private:
  static std::vector<StressCall> calls;
  static std::mutex dataMutex; // Mutex for protecting 'calls' data
  static const std::vector<std::string> types;
  static const std::vector<std::string> priorities;
  static const std::vector<std::string> addresses;
  static const std::vector<std::string> medicalStressCalls;
  static const std::vector<std::string> hazardStressCalls;
  static const std::vector<std::string> crimeStressCalls;
  static const std::vector<std::string> politicalStressCalls;
  static const std::vector<std::string> disasterStressCalls;
};

int get_random(int min, int max);

#endif

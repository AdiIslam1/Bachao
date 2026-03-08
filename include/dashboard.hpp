#ifndef DASHBOARD_HPP
#define DASHBOARD_HPP

#include <atomic>
#include <chrono>
#include <color.hpp>
#include <ctime>
#include <heroes.hpp>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

using namespace std;

class Dashboard {
private:
  class Message {
  private:
    string text;
    chrono::steady_clock::time_point startTime;

  public:
    Message(string msg) : text(msg), startTime(chrono::steady_clock::now()) {}

    string getText() const { return text; }

    bool isExpired() const {
      auto now = chrono::steady_clock::now();
      // 5 seconds message timer
      return chrono::duration_cast<chrono::seconds>(now - startTime).count() >=
             5;
    }
  };

  static vector<Message> messages;

  // Helper to extract inputs
  static void processInput(int callId, int heroId);

  // UI helpers
  static void renderInputArea();
  static void renderMessages();
  static void clearInputArea();
  static void clearMessageArea();

public:
  Dashboard() = default;
  static void start(atomic<bool> &isRunning);
};

#endif

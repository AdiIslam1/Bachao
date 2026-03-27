#include "console_utils.hpp"
#include <dashboard.hpp>
#include "stats.hpp"
#include <limits>
#include <sstream>
#include <unistd.h>

// IMPORTANT

// Terminal dimensions MUST be 155 x 50 or higher. Lower will break the output.

// Use ConsoleUtils::consoleMutex to synchronize console output

vector<Dashboard::Message> Dashboard::messages;

void Dashboard::processInput(int callId, int heroId) {
  lock_guard<mutex> lock(ConsoleUtils::consoleMutex);
  string msgText = "";
  // bool heroSavedYou = true;
  // Dispatch message for display
  // Implement message logic here (Handle out of bounds inputs too)
  try {
    Hero* hero = Hero::getHeroByIndex(heroId);
    string callType = StressCall::getCallType(callId);
    
    if (hero->getStatus() != "Available"){
      throw HeroUnavailableException(hero->getName(), hero->getStatus());
    }
    
    if (!hero->canHandle(callType)) {
      throw IncompatibleHeroException(hero->getHeroType(), callType);
    }
    
    if(hero->getStamina() < 25){
      throw HeroExhaustedException(hero->getName());
    }
    
    if(StressCall::resolveCall(callId)){
      Stats::addSuccess();
      hero->setStatus("On-Duty");
      hero->decreaseStamina(25); // Decrease stamina by 25 for taking a call, can be adjusted based on call severity/type
      // get how long it will to resolve the call.

      msgText = "Dispatching Hero ID " + to_string(heroId) + " to Call Serial " + to_string(callId);
      messages.push_back(Message(msgText));

      int resolutionTime = hero->getResolutionTime();
      msgText = Color::green("SUCCESS: ") + hero->getName() + " dispatched! ETA: " + to_string(resolutionTime) + "s.";
      messages.push_back(Message(msgText));

      // increase hero skill level after successful dispatch
      try {
          ++(*hero);
      } catch (const SkillBoundaryException& e) {
          messages.push_back(Message(Color::yellow(e.what())));
      }

      // Simulate the hero being on-duty for the resolution time, then set them back to available
      
      thread recoveryThread([hero, resolutionTime]() {
        
        // Wait for them to finish the job
        std::this_thread::sleep_for(std::chrono::seconds(resolutionTime));
        
        // They are tired, let them rest
        hero->setStatus("Resting");
        int restTime = hero->getStamina() <= 25 ? 15 : 8;
        std::this_thread::sleep_for(std::chrono::seconds(restTime));
        // Ready for action again!
        hero->setStatus("Available");
         
      });
      
      recoveryThread.detach();
      
    } else {
      Stats::addFailure();
      msgText = Color::red("ERROR: Failed to resolve Call ID " + to_string(callId) + " with Hero ID " + to_string(heroId) + "!");
      messages.push_back(Message(msgText));
    }
    
  } catch (const DispatchException& e) {
    messages.push_back(Message(Color::red(e.what())));
    Stats::addFailure();
  }
}

void Dashboard::renderInputArea() {
  // Lock console mutex
  {
    lock_guard<mutex> lock(ConsoleUtils::consoleMutex);
    if (cout.fail())
      cout.clear(); // Recover from stream errors
    stringstream ss;
    // Move to line 42
    ss << "\033[42;1H";
    for (int i = 0; i < 90; i++)
      ss << " ";        // Clear left side only (89 columns)
    ss << "\033[42;1H"; // Back to start of line
    ss << Color::bold("Enter Call ID & Hero ID: ");
    cout << ss.str();
    cout.flush();
  } // Extra scope for lock guard
}

void Dashboard::renderMessages() {
  lock_guard<mutex> lock(ConsoleUtils::consoleMutex);
  if (cout.fail())
    cout.clear();
  stringstream ss;
  ss << "\0337"; // Save cursor position

  // Remove expired messages
  auto it = messages.begin();
  while (it != messages.end()) {
    if (it->isExpired()) {
      it = messages.erase(it);
    } else {
      ++it;
    }
  }

  // Clear message area (lines 43-50)
  for (int i = 0; i < 8; i++) {
    ss << "\033[" << (43 + i) << ";1H";
    for (int j = 0; j < 90; j++)
      ss << " "; // Clear left side only (89 columns)
  }

  // Print active messages
  int row = 43;
  for (const auto &msg : messages) {
    ss << "\033[" << row++ << ";1H";
    ss << Color::green(
        msg.getText()); // Implementation needed to show different colours
    if (row > 50)
      break; // strict limit to row 50
  }
  ss << "\0338"; // Restore cursor position
  cout << ss.str();
  cout.flush();
}

void Dashboard::start(atomic<bool> &isRunning) {
  // Input Thread - Detached to allow continuous input without blocking main
  // logic
  thread inputThread([&isRunning]() {
    // Decouple cin from cout to prevent blocking/flushing races
    cin.tie(NULL);

    int callId, heroId;
    string line;

    // Loop continuously until isRunning is false, BUT process the last input
    // even if it flips false (for safely exiting the program)
    // Loop continuously until isRunning is false
    while (isRunning) {
      try {
        renderInputArea();

        if (getline(cin, line)) {
          stringstream ss(line);
          if (ss >> callId >> heroId) {
            // Only process logic if game is still running
            if (isRunning)
              processInput(callId, heroId);

            // Clear input line after processing
            {
              lock_guard<mutex> lock(ConsoleUtils::consoleMutex);
              cout << "\033[42;25H";
              for (int i = 0; i < 40; i++)
                cout << " ";
              cout << "\033[42;25H";
              cout.flush();
            }
          } else {
            // Invalid input handling
            lock_guard<mutex> lock(ConsoleUtils::consoleMutex);
            if (cout.fail())
              cout.clear();
            cout << "\033[42;25H";
            for (int i = 0; i < 40; i++)
              cout << " ";
            cout << "\033[42;25H";
            cout.flush();
          }
        } else {
          // Stream error or EOF
          if (cin.eof()) {
            // EOF is fatal for standard input usually, but we try to
            // recover/wait
            cin.clear(); // Clear flag?
            this_thread::sleep_for(chrono::milliseconds(500));
            continue;
          }

          // Bad bit or Fail bit
          cin.clear();
          // Ignore rest of line to prevent infinite read loop of bad char
          cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          this_thread::sleep_for(chrono::milliseconds(100));
        }
      } catch (const std::exception &e) {
        // Catch any random logic exceptions to keep thread alive
        // Log? cannot log to console.
        this_thread::sleep_for(chrono::milliseconds(100));
      }
    }
  });
  inputThread.detach();

  // Message Refresh Thread
  thread messageThread([&isRunning]() {
    while (isRunning) {
      renderMessages();
      this_thread::sleep_for(chrono::milliseconds(200));
    }
  });
  messageThread.detach();
}
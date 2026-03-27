# 🚨 BACHAO: Stress Calls Dispatch Terminal 🚨

![C++](https://img.shields.io/badge/C++-17-blue.svg?style=flat&logo=c%2B%2B)
![CMake](https://img.shields.io/badge/CMake-Build-success?style=flat&logo=cmake)
![Multithreaded](https://img.shields.io/badge/Architecture-Multithreaded-brightgreen)

Welcome to **BACHAO** (Bengali for "Save"), a high-stakes, real-time resource management and dispatch simulation built entirely in the C++ terminal! 

The city is in chaos, emergencies are popping up everywhere, and the clock is ticking. You have exactly **120 seconds** to manage your roster of heroes, dispatch them to active crises, and keep the city from falling apart. Do you have what it takes to get an "Outstanding Leadership" rating?

---

## 🎮 The Mission (Gameplay)

Unlike normal turn-based terminal games, **BACHAO runs in real-time**. While you are deciding who to send, the emergency timers are constantly ticking down! 

The screen is divided into three live-updating sectors:
1. **⬅️ Live Emergency Dashboard:** A ticking list of active stress calls (crimes, disasters, medical emergencies) with countdown timers. If a timer hits zero, the call expires, and people get hurt!
2. **➡️ Hero Roster:** Your list of available first responders. You must monitor their Skills, Status (Available, On-Duty, Resting), and **Stamina**.
3. **⬇️ Dispatch Command:** The input terminal where you make the calls. 

### 🦸‍♂️ Meet Your Heroes
You command a specialized task force. Sending the wrong hero to an emergency will result in a dispatch failure!
* 🚑 **Medics (Dr. Emily, Dr. James, Dr. Lisa):** Exclusively handle **Medical** emergencies.
* 🚓 **Police (Officer John, Officer Sarah, Officer Mike):** Armed and ready to handle **Crime** and **Political** unrest.
* 🚒 **Firefighters (Chief Tom, Captain Alex, Lt. Rachel):** Heavy-duty responders for **Hazards** and **Disasters**.

---

## ✨ Core Features

* ⏱️ **Real-Time Multithreading:** The game utilizes `std::thread` and `std::mutex` to independently render emergencies, track hero stamina, and read player input simultaneously without freezing the screen.
* 🧠 **Polymorphic OOP Logic:** Built with strict C++ inheritance. Heroes possess unique resolution times based on their `skillLevel` and utilize virtual functions to validate dispatch requests.
* 🔋 **Stamina Management System:** Heroes aren't machines. Dispatching a hero drains their stamina. Push them too hard, and they will be forced into a long recovery rest, leaving you short-handed!
* 🎨 **ANSI Escape UI:** A fully custom terminal engine that teleports the cursor and paints the terminal with color-coded alerts (Red for critical timers, Green for available heroes).
* 📊 **End-of-Shift Analytics:** When the 120-second shift ends, the game calculates your win percentage based on Generated, Resolved, and Expired calls to give you a final performance grade.

---

## 🚀 How to Play

### 1. Terminal Requirements
Because BACHAO uses a highly customized live UI, **your terminal window MUST be resized to at least 155 x 50 dimensions** or full-screen before running the game, otherwise the UI will break!

### 2. Dispatching Units
Look at the active calls on the left, and your available heroes on the right. 
In the command prompt at the bottom, type the `<Call ID>` followed by the `<Hero ID>` and press Enter.

**Example:**
To send Hero #4 (Dr. Emily) to Call #1 (A Medical Emergency), type:
```text
1 4
```

---

## 🏗️ Object-Oriented Programming (OOP) Concepts Utilized

BACHAO is engineered using a robust Object-Oriented C++ architecture. Here are the core concepts implemented throughout the codebase:

### 1. Encapsulation & Abstraction
* **Classes & Access Modifiers:** Foundational models like `StressCall`, `Hero`, `Stats`, and `Dashboard` use `private`, `protected`, and `public` specifiers to hide internal state (like a hero's true `stamina` or exact `status`) and expose only strictly validated getter and setter wrappers.
* **Nested Classes:** The `Message` class is safely encapsulated *inside* the private scope of the `Dashboard` class, abstracting away the timer strings mapped strictly for internal rendering logic.

### 2. Inheritance
* **Hierarchical Classification:** `Medic`, `Police`, and `Firefighter` publicly inherit from the base `Hero` class, inheriting members like `name`, `stamina`, and `status`, while keeping specific implementations separated.
* **Custom Exceptions:** Extensive OOP error handling is achieved via a dedicated `DispatchException` hierarchy (e.g. `InvalidHeroException`, `HeroExhaustedException`) that inherits directly from the standard `std::exception` class.

### 3. Polymorphism & Virtual Functions
* **Pure Virtual Methods:** The `Hero` base class enforces a uniform behavior contract through pure virtual functions like `virtual bool canHandle(const string& callType) const = 0` and `virtual int getResolutionTime() const = 0`.
* **Dynamic Binding:** A unified `vector<Hero*> heroList` array takes advantage of runtime polymorphism. The main dispatch loop seamlessly calls `hero->canHandle()` universally, and the C++ engine dynamically routes the query to the correct derived `Medic`, `Police`, or `Firefighter` method depending on the underlying object type.

### 4. Constructors and Destructors
* **Parameterized Constructors:** Objects are explicitly initialized via parameterized constructors utilizing member initializer lists (e.g. `Hero(...) : heroType(heroType), name("")...`). 
* **Virtual Destructors:** The `Hero` class provides a `virtual ~Hero() = default;` destructor ensuring that whenever polymorphic base pointers are deleted, the application properly cascades the memory destruction to the specific child objects without leaking.

### 5. File Handling
* The `Stats` static class seamlessly utilizes `std::ifstream` and `std::ofstream` specifically inside `Stats::saveHighScore()` and `Stats::printLeaderboard()`. This system persistently reads, truncates, overwrites, and organizes the Top 10 Highscore `highscore.txt` text file across multiple play sessions.

### 6. Robust Error Handling (try-catch)
* The entire command dispatch structure within `Dashboard::processInput()` prevents terminal halting using a massive `try-catch` wrapper. Illegal moves (like deploying exhausted responders) directly `throw` targeted `DispatchException` objects, which are safely caught and translated into visible red UI error logs dynamically rendering the payload of the overridden `.what()` method.

### 7. Constants and Statics
* **Constant keyword (`const`):** Emphasized aggressively in class getters (e.g. `string getHeroType() const`) to strongly guarantee state-safety inside loops, alongside `const std::vector` databases containing all randomly sourced locations.
* **Static Storage & Mutex Locks:** Singleton-patterned architectures utilizing `static` variables across `Stats` tracking arrays (`successCount`) alongside heavily `std::mutex` locked static routines protecting critical regions during the multi-threading loop.

### 8. Operator Overloading
* The `Hero` class provides custom implementations for the unary `operator++` and `operator--`. This allows the application to cleanly increment or decrement a hero's `skillLevel` using expressions like `++(*hero);` instead of traditional mutator functions, naturally enforcing min/max boundaries internally.

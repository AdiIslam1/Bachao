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

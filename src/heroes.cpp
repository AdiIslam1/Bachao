#include "heroes.hpp"
#include <sstream>

// Constructor
Hero::Hero(const string &heroType)
    : heroType(heroType), name(""), skillLevel(0), stamina(100), maxStamina(100), status("Available") {}

// ===================== VALIDATION FUNCTIONS =====================

void Hero::setSkillLevel(int newSkillLevel) {
    if (newSkillLevel < 1 || newSkillLevel > 100) {
        return;
    }
    skillLevel = newSkillLevel;
}

void Hero::setStatus(const string &newStatus) {
    if (newStatus != "Available" &&
        newStatus != "On-Duty" &&
        newStatus != "Resting" &&
        newStatus != "Dead") {
        return;
    }
    status = newStatus;
}

Hero& Hero::operator++() {
    if (skillLevel >= 100) {
        throw SkillBoundaryException(name, "maximum");
    }
    skillLevel++;
    return *this;
}

Hero& Hero::operator--() {
    if (skillLevel <= 1) {
        throw SkillBoundaryException(name, "minimum");
    }
    skillLevel--;
    return *this;
}

Hero* Hero::getHeroByIndex(int index) {
    int inputIndex = index;
    index--;
    if (index < 0 || index >= heroList.size()) {
        throw InvalidHeroException(inputIndex);
    }
    return heroList[index];
}

// ===================== THREAD FUNCTION =====================

void Hero::printHeroList(atomic<bool> &isRunning) {
    try {
        const int startCol = 100;

        while (isRunning) {
            stringstream ss;

            ss << "\0337";
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
                if (!hero) {
                    continue;
                }

                moveCursorSS(row++, startCol);
                ss << idx++ << ". ";

                string type = hero->getHeroType();
                if (type == "Medic")
                    ss << Color::red("[MEDIC]");
                else if (type == "Police")
                    ss << Color::blue("[POLICE]");
                else if (type == "Firefighter")
                    ss << Color::yellow("[FIREFIGHTER]");
                else
                    ss << "[UNKNOWN]";

                ss << " " << hero->getName();

                moveCursorSS(row++, startCol);
                ss << "   Skill Level: " << hero->getSkillLevel();

                string status = hero->getStatus();
                // Pad status string with spaces to overwrite previous longer statuses (e.g., "Available" vs "On-Duty")
                string displayStatus = status;
                while (displayStatus.length() < 9) displayStatus += " ";

                if (status == "Available")
                    ss << " | Status: " << Color::green(displayStatus);
                else if (status == "On-Duty")
                    ss << " | Status: " << Color::yellow(displayStatus);
                else if (status == "Resting")
                    ss << " | Status: " << Color::cyan(displayStatus);
                else if (status == "Dead")
                    ss << " | Status: " << Color::red(displayStatus);
                else
                    ss << " | Status: " << displayStatus;

                moveCursorSS(row++, startCol);
                ss << "--------------------------------";
            }

            ss << "\0338";

            {
                lock_guard<mutex> lock(ConsoleUtils::consoleMutex);
                if (cout.fail())
                    cout.clear();

                cout << ss.str();
                cout.flush();
            }

            this_thread::sleep_for(chrono::seconds(1));
        }
    }
    catch (const exception &e) {
        lock_guard<mutex> lock(ConsoleUtils::consoleMutex);
        cout << "\n[Hero Thread Error]: " << e.what() << endl;
    }
}

// ===================== HERO LIST INIT =====================

vector<Hero*> Hero::heroList = {
    new Police("Officer John", 3, "Available"),
    new Police("Officer Sarah", 4, "Available"),
    new Police("Officer Mike", 5, "Available"),

    new Medic("Dr. Emily", 3, "Available"),
    new Medic("Dr. James", 4, "Available"),
    new Medic("Dr. Lisa", 5, "Available"),

    new Firefighter("Chief Tom", 3, "Available"),
    new Firefighter("Captain Alex", 4, "Available"),
    new Firefighter("Lt. Rachel", 5, "Available")
};

// ===================== DERIVED CLASS IMPLEMENTATIONS =====================

Medic::Medic(const string &name, int skillLevel, const string &status)
    : Hero("Medic") {
    this->name = name;
    setSkillLevel(skillLevel);
    setStatus(status);
}

bool Medic::canHandle(const string& callType) const {
    return callType == "Medical";
}

int Medic::getResolutionTime() const {
    return max(5, 30 - skillLevel * 2);
}

// ---------------------

Police::Police(const string &name, int skillLevel, const string &status)
    : Hero("Police") {
    this->name = name;
    setSkillLevel(skillLevel);
    setStatus(status);
}

bool Police::canHandle(const string& callType) const {
    return callType == "Crime" || callType == "Political";
}

int Police::getResolutionTime() const {
    return max(5, 40 - skillLevel * 3);
}

// ---------------------

Firefighter::Firefighter(const string &name, int skillLevel, const string &status)
    : Hero("Firefighter") {
    this->name = name;
    setSkillLevel(skillLevel);
    setStatus(status);
}

bool Firefighter::canHandle(const string& callType) const {
    return callType == "Hazard" || callType == "Disaster";
}

int Firefighter::getResolutionTime() const {
    return max(5, 50 - skillLevel * 4);
}
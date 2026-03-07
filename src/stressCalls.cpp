#include <sstream>
#include <stressCalls.hpp>

// Define static members
std::vector<StressCall> StressCall::calls;
std::mutex StressCall::dataMutex;
const std::vector<std::string> StressCall::types = {
    "Medical", "Hazard", "Crime", "Political", "Disaster"};
const std::vector<std::string> StressCall::priorities = {
    "Low", "Medium", "High", "Very High", "Critical"};
const std::vector<std::string> StressCall::addresses = {
    "Plot 12, Road 4, Sector 7, Uttara",
    "House 45, Dhanmondi 27",
    "Block C, Banasree Main Road",
    "Shop 14, Bashundhara City Level 3",
    "Nikunja 2, Road 15, House 8",
    "Mohakhali Wireless Gate, near Mosque",
    "Tejgaon Industrial Area, near Nabisco Junction",
    "Farmgate Overbridge, East Side",
    "Mirpur 10, Block D, Lane 4",
    "Rampura Bridge, near TV Center",
    "Gulshan 2, Road 88, House 12",
    "Banani Block F, Road 11",
    "Khilgaon Taltola Market",
    "Malibagh Chowdhury Para",
    "Moghbazar Rail Crossing",
    "Kakrail Junction, near Nightingale",
    "Segunbagicha, Road 3, House 21",
    "Bijoy Sarani, near Rangs Bhaban",
    "Agargaon Taltola, near Passport Office",
    "Shewrapara, Metro Rail Pillar 240",
    "Kazipara, Main Road near Footbridge",
    "Mirpur 1, Sony Cinema Hall Lane",
    "Gabtoli Bus Terminal, Platform 4",
    "Shyamoli Square, Opposite Side",
    "Kalyanpur Bus Stand, Road 2",
    "Adabor 10, near Baitul Aman Mosque",
    "Mohammadpur Town Hall, Market Lane",
    "Lalmatia Block G, Road 4",
    "Hazaribagh Tannery Area, near Tannery Mor",
    "Kamrangirchar, Madrasa Road",
    "Old Dhaka, Lalbagh Fort Road",
    "Chawkbazar, near Shahi Mosque",
    "Sadarghat Terminal, Gate 2",
    "Patuatuly, Electronics Market Lane",
    "Kotwali, Johnson Road",
    "Armanitola, near Tara Masjid",
    "Wari, Rankin Street, House 5",
    "Gopibagh, near Railway Colony",
    "Motijheel C/A, behind Sonali Bank",
    "Dilkusha, near Bangabhaban Road",
    "Paltan Junction, near Baitul Mukarram",
    "Shahbagh, near PG Hospital Gate",
    "Dhaka University, Curzon Hall Area",
    "Azimpur Colony, Building 14",
    "New Market, Gate 4",
    "Elephant Road, Multiplan Center Lane",
    "Green Road, near Comfort Hospital",
    "Panthapath, near Square Hospital",
    "Kathalbagan, Free School Street",
    "Jigatola, near Japan Bangladesh Hospital",
    "Rayer Bazar, Graveyard Road",
    "Sankar Bus Stand, West Side",
    "Bosila, Garden City Road",
    "Keraniganj, near Babubazar Bridge",
    "Demra, Sarulia Bazar",
    "Jatrabari, near Flyover Entry",
    "Sayedabad Bus Terminal, Platform 1",
    "Dolaipar Junction",
    "Jurain Rail Crossing",
    "Postogola, near Buriganga Bridge",
    "Shantinagar, near Twin Towers",
    "Siddheshwari, near Girls College",
    "Baily Road, near Natok Shoroni",
    "Eskaton Garden, Road 2",
    "Karwan Bazar, near Kitchen Market",
    "Tejkunipara, Rail Line Side",
    "Nakhalpara, near PM Office Wall",
    "Cantonment, near Jahangir Gate",
    "Kafrul, near Police Station",
    "Vasantek, Block B, Road 1",
    "Matikata, near Flyover",
    "Manikdi, near Bazar Mosque",
    "Kalachandpur, near School Road",
    "Nadda, near Bashundhara Gate",
    "Kuril Bishwa Road, near Flyover Loop",
    "Joar Sahara, near Rail Station",
    "Khilkhet, near Regency Hotel",
    "Dumni, Purbachal Road Link",
    "Purbachal, Sector 4, Road 2",
    "Badda, near Link Road Bridge",
    "Merul Badda, DIT Project Road",
    "Aftabnagar, Block C, Road 1",
    "Mugdapara, near General Hospital",
    "Basabo, near Playground",
    "Goron, near Water Tank",
    "Nandipara, Bridge Road",
    "Madartek, near Chowrasta",
    "Donia, near AK School",
    "Kadirabad Housing, Mohammadpur",
    "Basila, near Bridge",
    "Pallabi, Extension Block E",
    "Duaripara, near Water Pump",
    "Rupnagar, Road 12, Mirpur",
    "Section 12, Mirpur Bus Stand",
    "Paikpara, near Govt Quarters",
    "Tolarbagh, Mirpur 1",
    "Ananda Nagar, Badda",
    "Shahzadpur, near Confidence Tower",
    "Satarkul, near School Road",
    "Baridhara DOHS, Road 5"};

const std::vector<std::string> StressCall::medicalStressCalls = {
    "Cardiac arrest",
    "Severe respiratory distress",
    "Unconscious patient",
    "Active labor emergency",
    "Suspected stroke",
    "Anaphylactic shock",
    "High fever with delirium",
    "Deep laceration",
    "Internal bleeding",
    "Seizure in progress",
    "Choking incident",
    "Severe head injury",
    "Sudden collapse",
    "Acute chest pain",
    "Third-degree burns",
    "Drug overdose",
    "Heat stroke",
    "Amputation accident",
    "Acute poisoning",
    "Fainting spell"};
const std::vector<std::string> StressCall::hazardStressCalls = {
    "Natural gas leak",
    "Downed live power line",
    "Chemical spill",
    "Electrical short circuit",
    "Toxic fume emission",
    "Structural wall crack",
    "Transformer explosion",
    "Building foundation shift",
    "Industrial refrigerant leak",
    "Elevator entrapment",
    "Sewerage backup",
    "Pressurized cylinder hiss",
    "Unstable billboard",
    "Main waterline burst",
    "Construction crane malfunction",
    "Radiological threat",
    "Basement flooding",
    "Open manhole hazard",
    "Leaning electric pole",
    "Corrosive acid spill"};
const std::vector<std::string> StressCall::crimeStressCalls = {
    "Armed robbery",         "Pedestrian snatching",
    "Physical assault",      "Burglary in progress",
    "Discharge of firearms", "Domestic violence",
    "Grand theft auto",      "Illegal assembly",
    "Public vandalism",      "Street harassment",
    "Attempted kidnapping",  "Narcotics distribution",
    "Pickpocketing",         "Commercial looting",
    "Home invasion",         "Cybersecurity breach",
    "Stabbing incident",     "Extortion attempt",
    "Mugging in progress",   "Illegal weapons possession"};
const std::vector<std::string> StressCall::politicalStressCalls = {
    "Road blockade",
    "Unannounced procession",
    "Vehicle arson",
    "Inter-party clash",
    "Barricade deployment",
    "Mass rally",
    "Vandalized party office",
    "Slogan chanting crowd",
    "Improvised explosive device",
    "Police cordon breach",
    "Hunger strike",
    "Tire burning",
    "Projectile throwing",
    "VIP motorcade delay",
    "Unauthorized public speech",
    "Religious-political gathering",
    "Civil disobedience",
    "Government building sit-in",
    "Political graffiti spree",
    "Curfew violation"};
const std::vector<std::string> StressCall::disasterStressCalls = {
    "Flash flood",
    "High-rise building fire",
    "Earthquake tremor damage",
    "Massive tree fall",
    "Monsoon waterlogging",
    "Thunderstorm casualty",
    "Industrial warehouse fire",
    "Building tilt/subsidence",
    "Urban storm damage",
    "Slum fire outbreak",
    "Lightning strike",
    "Cyclonic wind damage",
    "Riverbank erosion",
    "Submerged roadway",
    "Garment factory fire",
    "Tornado debris",
    "Bridge structural failure",
    "Mudslide risk",
    "Massive debris blockage",
    "Emergency shelter overflow"};

int get_random(int min, int max) {
  // 'static' ensures the engine is initialized only once
  static std::random_device rd;
  static std::mt19937 gen(rd());

  std::uniform_int_distribution<int> dist(min, max);
  return dist(gen);
}

void StressCall::generate(int level) {
  // LOCK
  std::lock_guard<std::mutex> lock(dataMutex);
  for (int i = 0; i < level; i++) {
    int typeNo = get_random(0, 4);
    int activityNo = get_random(0, 19);
    int addressNo = get_random(0, 99);
    int priorityNo = get_random(0, 4);

    string type = types[typeNo];
    string activity = "";

    if (type == "Medical")
      activity = medicalStressCalls[activityNo];
    else if (type == "Hazard")
      activity = hazardStressCalls[activityNo];
    else if (type == "Crime")
      activity = crimeStressCalls[activityNo];
    else if (type == "Political")
      activity = politicalStressCalls[activityNo];
    else if (type == "Disaster")
      activity = disasterStressCalls[activityNo];

    string address = addresses[addressNo];
    string priority = priorities[priorityNo];

    calls.push_back(
        StressCall(type, activity, address, priority, get_random(30, 60)));
  }
}

void StressCall::tickAll() {
  std::lock_guard<std::mutex> lock(dataMutex);
  for (auto &call : calls) {
    call.tick();
  }
}

bool StressCall::isEmpty() {
  std::lock_guard<std::mutex> lock(dataMutex);
  return calls.empty();
}

void StressCall::printCallList(atomic<bool> &isRunning) {
  while (isRunning || !calls.empty()) {
    stringstream ss;
    {
      // DATA LOCK: Logic and Buffer Building
      std::lock_guard<std::mutex> lock(dataMutex);

      ss << "\0337"; // Save cursor position
      ss << ConsoleUtils::getCursorCode(1, 1);

      // Build the frame in memory
      for (int row = 1; row <= 40; row++) {
        ss << "\033[" << row << ";1H";
        for (int i = 0; i < 90; i++)
          ss << " ";
      }

      // Move to top
      ss << "\033[1;1H";
      ss << Color::cyan("======================================") << "\n";
      ss << Color::yellow(Color::bold(" BACHAO Stress Calls Dispatch Terminal"))
         << "\n";
      ss << Color::cyan("======================================") << "\n";
      ss << "=== LIVE EMERGENCY DASHBOARD ===" << "\n";
      ss << "Active Calls: " << calls.size() << "\n";
      ss << "--------------------------------" << "\n";

      // We have printed 6 lines so far. Next is line 7.
      int currentRow = 7;

      auto it = calls.begin();
      int idx = 1;

      while (it != calls.end()) {
        it->tick();

        if (currentRow > 40) {
          if (it->getTimeLeft() <= 0)
            it = calls.erase(it);
          else
            ++it;
          continue;
        }

        if (it->getTimeLeft() <= 0) {
          // Printing expired call logic
          // This usually takes 3 lines: msg + divider + newline
          if (currentRow + 2 <= 40) {
            ss << "\033[" << currentRow++ << ";1H";
            string msg = Color::red("[EXPIRED/MISSED] ") + it->getType() +
                         " at " + it->getAddress();
            ss << msg;
            ss << "\033[" << currentRow++ << ";1H";
            ss << "--------------------------------";
          }
          it = calls.erase(it);
        } else {

          if (currentRow + 3 <= 40) {
            ss << "\033[" << currentRow++ << ";1H";
            ss << idx++ << ". ";
            string type = it->getType();
            if (type == "Medical")
              ss << Color::red("[MEDICAL]");
            else if (type == "Hazard")
              ss << Color::yellow("[HAZARD]");
            else if (type == "Crime")
              ss << Color::blue("[CRIME]");
            else if (type == "Political")
              ss << Color::magenta("[POLITICAL]");
            else if (type == "Disaster")
              ss << Color::cyan("[DISASTER]");

            ss << " " << it->getActivity();

            ss << "\033[" << currentRow++ << ";1H";
            ss << "   Loc: " << it->getAddress();

            int t = it->getTimeLeft();
            string timeStr = " [T-" + to_string(t) + "s]";

            if (t <= 5)
              ss << Color::bold(Color::red(timeStr));
            else if (t <= 15)
              ss << Color::yellow(timeStr);
            else
              ss << Color::green(timeStr);

            ss << "\033[" << currentRow++ << ";1H";
            ss << "--------------------------------";
          }
          ++it;
        }
      }
      ss << "\0338"; // Restore cursor position
    } // End dataMutex lock

    // CONSOLE LOCK: Actual Printing
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
// reads the type of call. Returns string.
string StressCall::getCallType(int index) {
  std::lock_guard<std::mutex> lock(dataMutex);
  index--;
  if (index < 0 || index >= calls.size())
    return "";
  return calls[index].getType();
}

// resolves the call at index. Successfull if hero type matches call type. Returns true if resolved, false otherwise.
bool StressCall::resolveCall(int index) {
  std::lock_guard<std::mutex> lock(dataMutex);
  index--;
  if (index < 0 || index >= calls.size())
    return false;
  calls.erase(calls.begin() + index);
  return true;
}

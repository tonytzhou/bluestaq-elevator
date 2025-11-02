// Engineer.cpp
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cctype>
#include <iostream>
#include <limits>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <algorithm>

#include "headers/Engineer.h"
#include "headers/Elevator.h"

using namespace std;

namespace
{
   struct Session
   {
      Elevator elevator;
      mutex m;
      condition_variable cv;
      atomic<bool> running{true};
      atomic<bool> haveWork{false};
      Direction travel{IDLE};
   };

   void printHelp(int totalFloors)
   {
      cout << "\nEngineer input commands (case-insensitive):\n"
           << "  <floor> U      : Outside request at <floor> wanting to go Up\n"
           << "  <floor> D      : Outside request at <floor> wanting to go Down\n"
           << "  <floor> N      : Inside-elevator request to go to <floor>\n"
           << "  P              : Print elevator status\n"
           << "  H              : Help (this menu)\n"
           << "  Q              : Quit engineer mode\n"
           << "Notes: floors are 1.." << totalFloors
           << ". Input accepted while elevator is moving.\n\n";
   }

   void travelDots(int steps = 3, int ms = 600)
   {
      for (int i = 0; i < steps; ++i)
      {
         this_thread::sleep_for(chrono::milliseconds(ms));
         cout << ".";
         cout.flush();
      }
      cout << "\n";
   }

   int parseFloor(const string &tok)
   {
      if (tok.empty())
         return -1;
      for (unsigned char c : tok)
         if (!isdigit(c))
            return -1;
      try
      {
         return stoi(tok);
      }
      catch (...)
      {
         return -1;
      }
   }

   bool normalizeTokens(string &a, string &b)
   {
      if (!b.empty())
         return true;
      if (a.empty())
         return false;
      unsigned char last = static_cast<unsigned char>(a.back());
      if (isalpha(last))
      {
         string digits = a.substr(0, a.size() - 1);
         if (!digits.empty() &&
             all_of(digits.begin(), digits.end(),
                    [](unsigned char c)
                    { return isdigit(c); }))
         {
            b = string(1, static_cast<char>(toupper(last)));
            a = digits;
            return true;
         }
      }
      return false;
   }

   void worker(Session *s)
   {
      while (s->running.load())
      {
         unique_lock<mutex> lk(s->m);
         s->cv.wait_for(lk, chrono::milliseconds(100), [&]
                        { return !s->running.load() || s->haveWork.load() || s->travel != IDLE; });
         if (!s->running.load())
            break;

         if (s->travel == IDLE)
         {
            lk.unlock();
            this_thread::sleep_for(chrono::milliseconds(60));
            continue;
         }

         int before = s->elevator.getCurrentFloor();

         if (s->travel == UP)
         {
            s->elevator.step(UP);
            int after = s->elevator.getCurrentFloor();
            if (after == before)
               s->travel = DOWN;
         }
         else
         {
            s->elevator.step(DOWN);
            int after = s->elevator.getCurrentFloor();
            if (after == before)
               s->travel = UP;
         }

         int dest = s->elevator.getCurrentFloor();
         bool moved = (dest != before);

         s->haveWork.store(false);
         lk.unlock();

         if (moved)
         {
            if (dest > before)
               cout << "Elevator moving up from " << before << " to " << dest << " ";
            else
               cout << "Elevator moving down from " << before << " to " << dest << " ";
            travelDots();

            {
               lock_guard<mutex> g(s->m);
               s->elevator.openDoors();
               s->elevator.closeDoors();
            }
         }
         else
         {
            this_thread::sleep_for(chrono::milliseconds(120));
         }
      }
   }

   void inputLoop(Session *s)
   {
      printHelp(s->elevator.getTotalFloors());

      string a, b;
      while (s->running.load())
      {
         cout << "> ";
         if (!(cin >> a))
         {
            s->running.store(false);
            s->cv.notify_all();
            break;
         }

         if (a.size() == 1)
         {
            char cmd = static_cast<char>(toupper(static_cast<unsigned char>(a[0])));
            if (cmd == 'Q')
            {
               s->running.store(false);
               s->cv.notify_all();
               break;
            }
            if (cmd == 'P')
            {
               lock_guard<mutex> g(s->m);
               cout << "[Status] Current floor: " << s->elevator.getCurrentFloor()
                    << " / Total floors: " << s->elevator.getTotalFloors() << "\n";
               continue;
            }
            if (cmd == 'H')
            {
               printHelp(s->elevator.getTotalFloors());
               continue;
            }
         }

         b.clear();
         if (!normalizeTokens(a, b))
         {
            if (!(cin >> b))
            {
               cout << "Expected a second token (U/D/N). Examples: '5 U' or '5U'. Type H for help.\n";
               cin.clear();
               continue;
            }
         }

         int floor = parseFloor(a);
         if (floor == -1)
         {
            cout << "Couldn't parse a floor from '" << a << "'. Use e.g. '5 U' or '5U'.\n";
            continue;
         }
         if (floor < 1 || floor > s->elevator.getTotalFloors())
         {
            cout << "Invalid floor. Must be 1.." << s->elevator.getTotalFloors() << ".\n";
            continue;
         }

         char kind = static_cast<char>(toupper(static_cast<unsigned char>(b[0])));
         {
            lock_guard<mutex> g(s->m);
            if (kind == 'N')
            {
               s->elevator.requestFloor(floor);
               if (s->travel == IDLE)
               {
                  int cur = s->elevator.getCurrentFloor();
                  s->travel = (floor > cur) ? UP : DOWN;
               }
               cout << "Queued inside request to floor " << floor << ".\n";
            }
            else if (kind == 'U' || kind == 'D')
            {
               Direction d = (kind == 'U') ? UP : DOWN;
               s->elevator.outsideRequest(floor, d);
               if (s->travel == IDLE)
                  s->travel = d;
               cout << "Queued outside request at floor " << floor
                    << " wanting to go " << (d == UP ? "Up" : "Down") << ".\n";
            }
            else
            {
               cout << "Unknown second token '" << b << "'. Use U/D/N. Type H for help.\n";
               continue;
            }
            s->haveWork.store(true);
         }
         s->cv.notify_one();
      }
   }

} // namespace

void EngineerMode()
{
   cout << "You have selected Engineer Mode.\n";
   cout << "Enter the total number of floors for the elevator (default 20): ";
   int numFloors = 20;
   if (!(cin >> numFloors) || numFloors < 1)
   {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      numFloors = 20;
   }

   Session s;
   {
      lock_guard<mutex> g(s.m);
      s.elevator = Elevator(numFloors);
   }

   cout << "Engineer Mode started with " << numFloors
        << " floors. Type H for commands. (Interactive Mode remains unchanged.)\n";

   thread tWorker(worker, &s);
   thread tInput(inputLoop, &s);

   tInput.join();
   s.running.store(false);
   s.cv.notify_all();
   tWorker.join();

   cout << "Exiting Simulation. Goodbye!\n";
}

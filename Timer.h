#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <unordered_map>
#include <atomic>

class Timer {
private:
  std::unordered_map<std::thread*, std::atomic<bool>> intervals;
public:
  template<typename Func>
  void delay(Func fn, int delay) {
    std::thread([fn, delay]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(delay));
      fn();
    }).detach();
  }

  template<typename Func>
  std::thread* interval(Func fn, int inter) {
    // Créer le thread sans le démarrer immédiatement
      std::thread* th = new std::thread();

      // Démarrer le thread et passer son propre pointeur à la lambda
      *th = std::thread([this, fn, inter, th]() {
          while (intervals[th]) {
              std::this_thread::sleep_for(std::chrono::milliseconds(inter));
              fn();
          }
      });
      intervals[th] = true;
      return th;
  }

  void stop(std::thread* th) {
    intervals[th] = false;
    if (th->joinable()) {
      th->join();
    }
    delete th;
    intervals.erase(th);
  }

  void stopAll() {
     for (auto& pair : intervals) {
          stop(pair.first);
     }
  }

  ~Timer() {
    stopAll(); 
  }

};

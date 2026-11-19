// —————— headers ————————————————————————— //
#include "game_timer.h"
// —————— headers ————————————————————————— //



// —————— global vars ————————————————————— //
unsigned long last_time = 0;               // time in-between change
int minutes = 0;                           // in-game timer minutes
int seconds = 0;                           // in-game timer seconds
TimeState time_state = TimeState::STOPPED; // status of timer
// —————— global vars ————————————————————— //



// |————————————————————————————————————————————| //
// |————| RETURN FUNCTIONS |————————————————————| //

int GetSeconds() { return seconds; }
int GetMinutes() { return minutes; }
bool NoTime() { return (minutes <= 0) && (seconds <= 0); }
bool TimeRunning() { return time_state == TimeState::PLAYING; }
bool TimeStopped() { return time_state == TimeState::STOPPED; }
bool TimePaused() { return time_state == TimeState::PAUSED; }

// |————| RETURN FUNCTIONS |————————————————————| //
// |————————————————————————————————————————————| //



// |—————————————————————————————————————————————| //
// |————| TIMER FUNCTIONS |——————————————————————| //

void PlayTimer()
{
  if (TimeRunning()) return;
  time_state = TimeState::PLAYING;
}

void PauseTimer()
{
  if (TimePaused()) return;
  time_state = TimeState::PAUSED;
}

void StopTimer()
{
  if (TimeStopped()) return;
  time_state = TimeState::STOPPED;
  minutes = 0;
  seconds = 0;
}

void UpdateTime(bool count_up)
{
  if (TimeRunning()) {
    const unsigned long ms = millis(),
                        diff_ms = (ms - last_time);
    if (diff_ms >= TO_SECONDS(1)) {
      last_time = ms;
      if (!count_up) {
        const bool out_of_seconds = (seconds <= 0),
                   has_minutes    = (minutes > 0),
                   minute_reset   = (has_minutes && out_of_seconds);
        minutes = minute_reset ? minutes - 1 : minutes;
        seconds = minute_reset ? 59          : seconds - 1;
        // DEBUG_Time();
        if (NoTime()) { StopTimer(); }
      } else {
        
      }
    }
  }
}

void UpdateTimer(int mins, int secs)
{
  int mTemp = minutes + mins;
  int sTemp = seconds + secs;
  while (sTemp >= 60) {
    mTemp++;
    sTemp %= 60;
  }
  minutes = mTemp;
  seconds = sTemp;
}

// |————| TIMER FUNCTIONS |——————————————————————| //
// |—————————————————————————————————————————————| //
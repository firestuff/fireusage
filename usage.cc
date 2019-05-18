#include <glog/logging.h>
#include <iomanip>

#include "usage.h"

namespace fireusage {

namespace {

std::chrono::nanoseconds TvToNs(const timeval& tv) {
  return std::chrono::seconds(tv.tv_sec) +
         std::chrono::microseconds(tv.tv_usec);
}

}  // namespace

void UsageTracker::AddEvents(uint64_t num) { events_ += num; }

void UsageTracker::AddEvent() { events_ += 1; }

void UsageTracker::Start() {
  CHECK(!running_);
  running_ = true;

  PCHECK(getrusage(RUSAGE_THREAD, &start_usage_) == 0);
  start_time_ = std::chrono::steady_clock::now();
}

void UsageTracker::Stop() {
  CHECK(running_);
  running_ = false;

  const auto end_time = std::chrono::steady_clock::now();

  rusage end_usage;
  PCHECK(getrusage(RUSAGE_THREAD, &end_usage) == 0);

  wall_time_ += end_time - start_time_;
  user_time_ += TvToNs(end_usage.ru_utime) - TvToNs(start_usage_.ru_utime);
  sys_time_ += TvToNs(end_usage.ru_stime) - TvToNs(start_usage_.ru_stime);
  vol_ctxt_sw_ += end_usage.ru_nvcsw - start_usage_.ru_nvcsw;
  invol_ctxt_sw_ += end_usage.ru_nivcsw - start_usage_.ru_nivcsw;
}

void UsageTracker::Log(const std::string_view& title) {
  if (!title.empty()) {
    // Need to do this here so --vmodule works properly
    VLOG(1) << title << ":";
  }

  VLOG(1) << "\t               events: " << std::setw(19) << std::setfill(' ')
          << events_;
  VLOG(1) << "\t            wall time: " << std::setw(19) << std::setfill(' ')
          << wall_time_.count() << "ns";
  VLOG(1) << "\t            user time: " << std::setw(19) << std::setfill(' ')
          << user_time_.count() << "ns ("
          << ((user_time_ * 100) / (user_time_ + sys_time_)) << "%)";
  VLOG(1) << "\t             sys time: " << std::setw(19) << std::setfill(' ')
          << sys_time_.count() << "ns ("
          << ((sys_time_ * 100) / (user_time_ + sys_time_)) << "%)";
  VLOG(1) << "\t          vol ctxt sw: " << std::setw(19) << std::setfill(' ')
          << vol_ctxt_sw_;
  VLOG(1) << "\t        invol ctxt sw: " << std::setw(19) << std::setfill(' ')
          << invol_ctxt_sw_;
  if (events_) {
    VLOG(1) << "\t    wall time / event: " << std::setw(19) << std::setfill(' ')
            << (wall_time_ / events_).count() << "ns";
    VLOG(1) << "\t    user time / event: " << std::setw(19) << std::setfill(' ')
            << (user_time_ / events_).count() << "ns";
    VLOG(1) << "\t     sys time / event: " << std::setw(19) << std::setfill(' ')
            << (sys_time_ / events_).count() << "ns";
    VLOG(1) << "\t  vol ctxt sw / event: " << std::setw(19) << std::setfill(' ')
            << (vol_ctxt_sw_ / events_);
    VLOG(1) << "\tinvol ctxt sw / event: " << std::setw(19) << std::setfill(' ')
            << (invol_ctxt_sw_ / events_);
  }
}

}  // namespace fireusage

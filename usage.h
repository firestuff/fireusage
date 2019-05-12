#pragma once

#include <chrono>
#include <string>
#include <sys/resource.h>

namespace fireusage {

class UsageTracker {
  public:
	void AddEvents(uint64_t num);
	void AddEvent();

	void Start();
	void Stop();
	void Log();

  private:
	bool running_ = false;
	uint64_t events_ = 0;
	std::chrono::nanoseconds wall_time_;
	std::chrono::nanoseconds user_time_;
	std::chrono::nanoseconds sys_time_;
	uint64_t vol_ctxt_sw_ = 0;
	uint64_t invol_ctxt_sw_ = 0;

	std::chrono::steady_clock::time_point start_time_;
	rusage start_usage_;
};

} // namespace fireusage

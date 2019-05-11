#include <glog/logging.h>
#include <iomanip>

#include "usage.h"

namespace fireusage {

namespace {

std::chrono::nanoseconds TvToNs(const timeval& tv) {
	return std::chrono::seconds(tv.tv_sec) + std::chrono::microseconds(tv.tv_usec);
	
}

} // namespace

void UsageTracker::AddEvents(uint64_t num) {
	events_ += num;
}

void UsageTracker::AddEvent() {
	events_ += 1;
}

void UsageTracker::Start() {
	CHECK(!running_);
	running_ = true;

	PCHECK(getrusage(RUSAGE_THREAD, &start_usage_) == 0);
	start_time_ = std::chrono::steady_clock::now();
}

void UsageTracker::Stop() {
	CHECK(running_);
	running_ = false;

	const auto end_time_ = std::chrono::steady_clock::now();

	rusage end_usage_;
	PCHECK(getrusage(RUSAGE_THREAD, &end_usage_) == 0);

	wall_time_ += end_time_ - start_time_;
	user_time_ += TvToNs(end_usage_.ru_utime) - TvToNs(start_usage_.ru_utime);
	sys_time_ += TvToNs(end_usage_.ru_stime) - TvToNs(start_usage_.ru_stime);
}

void UsageTracker::Log() {
	LOG(INFO) << "usage:";
	LOG(INFO) << "\tevents:            " << std::setw(20) << std::setfill(' ') << events_;
	LOG(INFO) << "\twall time:         " << std::setw(20) << std::setfill(' ') << wall_time_.count() << "ns";
	LOG(INFO) << "\tuser time:         " << std::setw(20) << std::setfill(' ') << user_time_.count() << "ns";
	LOG(INFO) << "\tsys time:          " << std::setw(20) << std::setfill(' ') << sys_time_.count() << "ns";
	LOG(INFO) << "\twall time / event: " << std::setw(20) << std::setfill(' ') << (wall_time_ / events_).count() << "ns";
	LOG(INFO) << "\tuser time / event: " << std::setw(20) << std::setfill(' ') << (user_time_ / events_).count() << "ns";
	LOG(INFO) << "\tsys time / event:  " << std::setw(20) << std::setfill(' ') << (sys_time_ / events_).count() << "ns";
}

} // namespace fireusage

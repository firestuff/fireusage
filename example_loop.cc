#include <gflags/gflags.h>
#include <glog/logging.h>

#include "usage.h"

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  fireusage::UsageTracker tracker;
  tracker.Start();
  for (int i = 0; i < 100000000; ++i) {
    tracker.AddEvent();
    for (int _ = 0; _ < 100000; ++_) {
    }
  }
  tracker.Stop();
  tracker.Log();

  gflags::ShutDownCommandLineFlags();
  google::ShutdownGoogleLogging();
}

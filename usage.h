#pragma once

#include <string>

namespace fireusage {

class UsageTracker {
  public:
	UsageTracker(const std::string_view& plural_resource_name);

  private:
	const std::string plural_resource_name_;
};

} // namespace fireusage

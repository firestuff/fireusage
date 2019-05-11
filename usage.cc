#include "usage.h"

namespace fireusage {

UsageTracker::UsageTracker(const std::string_view& plural_resource_name)
	: plural_resource_name_(plural_resource_name) {
}

} // namespace fireusage

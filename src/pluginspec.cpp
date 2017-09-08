#include "plugins/pluginspec.hpp"
#include "pluginspec_p.hpp"
#include <string>

PLUGINS_NS_BEGIN

bool PluginDependency::operator==(const PluginDependency &other) const {
  return name == other.name && version == other.version && type == other.type;
}

static std::string typeString(PluginDependency::Type type) {
  switch (type) {
  case PluginDependency::Optional:
    return std::string(", optional");
  case PluginDependency::Test:
    return std::string(", test");
  case PluginDependency::Required:
  default:
    return std::string();
  }
}

std::string PluginDependency::toString() const {
  return name + " (" + version + typeString(type) + ")";
}

/*
namespace internal {

class PluginSpecPrivate {

public:
  PluginSpecPrivate() {}

  std::string name;
  std::string version;
};
} // namespace internal*/

PluginSpec::PluginSpec() : d(new internal::PluginSpecPrivate(this)) {}

PluginSpec::~PluginSpec() {}

std::string PluginSpec::name() const { return d->name; }

std::string PluginSpec::version() const { return d->name; }

PLUGINS_NS_END

#include "plugins/pluginspec.hpp"
#include "pluginspec_p.hpp"
#include <string>

PLUGINS_NS_BEGIN

bool PluginDependency::operator==(const PluginDependency &other) const {
  return name == other.name && version == other.version && type == other.type;
}

bool PluginDependency::operator<(const PluginDependency &other) const {
  return name < other.name && version < other.version && type < other.type;
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

std::string PluginSpec::version() const { return d->version; }

bool PluginSpec::has_error() const { return d->has_error; }

std::string PluginSpec::error_string() const { return ""; }

PluginSpec::State PluginSpec::state() const { return d->state; }

std::vector<PluginDependency> PluginSpec::dependencies() const {
  return d->dependencies;
}

bool PluginSpec::provides(const std::string &name, const std::string &version) {

}

PLUGINS_NS_END

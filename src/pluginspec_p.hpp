#pragma once
#include <plugins/defines.hpp>
#include <plugins/pluginspec.hpp>
PLUGINS_NS_BEGIN

namespace internal {

class PluginSpecPrivate {
public:
  PluginSpecPrivate();

  bool report_error(const std::string &msg);

  PluginLoader loader;

  std::string name;
  std::string version;

  std::vector<PluginDependency> dependencies;

  PluginSpec::State state = PluginSpec::Invalid;
  std::string error_string;
  bool has_error = false;
};
} // namespace internal

PLUGINS_NS_END

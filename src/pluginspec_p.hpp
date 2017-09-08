#pragma once
#include <plugins/defines.hpp>
#include <plugins/pluginspec.hpp>

PLUGINS_NS_BEGIN

namespace internal {

class PluginSpecPrivate {
public:
  PluginSpecPrivate(PluginSpec *spec);

  bool report_error(const std::string &msg);
  bool read(const json &metadata);

  std::string name;
  std::string version;

  std::vector<PluginDependency> dependencies;
  PluginSpec *q;
  PluginSpec::State state = PluginSpec::Invalid;
  // std::string error_string;
  PluginSpec::Error error = PluginSpec::Valid;
  bool has_error = false;
};
} // namespace internal

PLUGINS_NS_END

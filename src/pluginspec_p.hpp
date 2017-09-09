#pragma once
#include <plugins/defines.hpp>
#include <plugins/iplugin.hpp>
#include <plugins/pluginspec.hpp>

PLUGINS_NS_BEGIN

namespace internal {

class PluginSpecPrivate {
public:
  PluginSpecPrivate(PluginSpec *spec);

  bool report_error(const std::string &msg);
  bool read(const json &metadata);
  bool resolveDependencies(const std::vector<PluginSpec *> &specs);
  bool initializeExtensions();
  bool initializePlugin();
  bool loadLibrary();

  void kill();
  IPlugin::ShutdownFlag stop();

  void enableDependenciesIndirectly();

  std::string name;
  std::string version;

  std::vector<std::string> arguments;
  std::vector<PluginDependency> dependencies;
  std::map<PluginDependency, PluginSpec *> dependencySpecs;
  PluginSpec *q;
  IPlugin *plugin = nullptr;
  PluginSpec::State state = PluginSpec::Invalid;
  std::string error_string;
  PluginSpec::Error error = PluginSpec::Valid;
  bool enabledIndirectly;
  bool has_error = false;
};

} // namespace internal

PLUGINS_NS_END

#pragma once
#include <plugins/defines.hpp>
#include <memory>
#include <spdlog/logger.h>
#include <set>

PLUGINS_NS_BEGIN

class PluginSpec;

namespace internal {
class PluginManagerPrivate {
public:
  PluginManagerPrivate(std::shared_ptr<spdlog::logger> l) : logger(l) {}

  void loadPlugins();
  void loadPlugin(PluginSpec *spec, PluginSpec::State state);
  void readPlugin(const std::string &path);
  void readPlugins();
  void resolveDependencies();

  std::set<std::string> search_paths;
  std::vector<IPluginProvider *> providers;
  std::vector<PluginSpec *> specs;
  std::shared_ptr<spdlog::logger> logger;

private:
  bool loadQueue(PluginSpec *spec, std::vector<PluginSpec *> &queue,
                 std::vector<PluginSpec *> &circularityCheckQueue);
  std::vector<PluginSpec *> loadQueue();
};


PLUGINS_NS_END

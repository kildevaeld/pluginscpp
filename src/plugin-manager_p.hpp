#pragma once
#include <plugins/defines.hpp>
#include <memory>
#include <spdlog/logger.h>
#include <set>
#include <plugins/pluginspec.hpp>

PLUGINS_NS_BEGIN

class IPluginProvider;


namespace internal {

class PluginManagerPrivate {
public:
  PluginManagerPrivate(std::shared_ptr<spdlog::logger> l) : logger(l) {}

  void loadPlugins();
  void initializePlugins();
  void loadPlugin(PluginSpec *spec, PluginSpec::State state);
  void readPlugin(const std::string &path);
  void readPlugins();
  void resolveDependencies();

  void shutdown();

  std::set<std::string> search_paths;
  std::vector<IPluginProvider *> providers;
  std::vector<PluginSpec *> specs;
  std::shared_ptr<spdlog::logger> logger;

private:
  bool loadQueue(PluginSpec *spec, std::vector<PluginSpec *> &queue,
                 std::vector<PluginSpec *> &circularityCheckQueue);
  
  void profilingReport(const char *what, const PluginSpec *spec = 0);

  void stopAll();
  void deleteAll();


  std::vector<PluginSpec *> loadQueue();
};

}

PLUGINS_NS_END

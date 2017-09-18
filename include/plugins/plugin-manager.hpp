#pragma once
#include "defines.hpp"
#include <memory>
//#include <plugins/pluginspec.hpp>
#include <any++.h>
#include <string>

namespace spdlog {
class logger;
}

PLUGINS_NS_BEGIN

namespace internal {
class PluginManagerPrivate;
}

class IPluginProvider;

class PluginManager {

public:
  PluginManager(std::shared_ptr<spdlog::logger> logger);
  PluginManager(const PluginManager &other) = delete;
  ~PluginManager();

  static PluginManager *instance() {}
  static void addObject(const Any &obj) {}

  void addSearchPath(const std::string &path);

  void loadPlugins();
  void initializePlugins();

  void registerProvider(IPluginProvider *provider);

  void shutdown();

protected:
  // void loadPlugin(const std::string &path);
  // void loadPlugin(PluginSpec *spec, PluginSpec::State state);

private:
  std::unique_ptr<internal::PluginManagerPrivate> d;
};

PLUGINS_NS_END

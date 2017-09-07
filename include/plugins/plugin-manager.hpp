#pragma once
#include "defines.hpp"
#include <memory>
#include <string>

PLUGINS_NS_BEGIN

namespace internal {
class PluginManagerPrivate;
}

class IPluginProvider;

class PluginManager {

public:
  PluginManager();
  PluginManager(const PluginManager &other) = delete;
  ~PluginManager();

  void addSearchPath(const std::string &path);
  void loadPlugins();

  void registerProvider(IPluginProvider *provider);

protected:
  void loadPlugin(const std::string &path);

private:
  std::unique_ptr<internal::PluginManagerPrivate> d;
};

PLUGINS_NS_END

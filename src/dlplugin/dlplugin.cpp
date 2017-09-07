#include <iostream>
#include <plugins/dlplugin/dlplugin.hpp>
#include <plugins/dlplugin/dlpluginloader.hpp>
#include <plugins/dlplugin/dlpluginspec.hpp>

PLUGINS_NS_BEGIN

PluginSpec *DLPluginManager::load(const std::string &path) {
  // std::cout << "PROVIDER" << path << std::endl;

  PluginLoader plugin(path);

  if (plugin.is_valid()) {
    return new DLPluginSpec(std::move(plugin));
  }

  return nullptr;
}

PLUGINS_NS_END

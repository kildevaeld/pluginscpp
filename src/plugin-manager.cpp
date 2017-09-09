#include "plugins/plugin-manager.hpp"
#include "helpers.hpp"
#include "plugin-manager_p.hpp"
#include "plugins/iplugin-provider.hpp"
#include "pluginspec_p.hpp"
#include <iostream>
#include <plugins/pluginspec.hpp>
#include <set>
#include <spdlog/spdlog.h>
#include <tinydir.h>
#include <vector>

PLUGINS_NS_BEGIN

PluginManager::PluginManager(std::shared_ptr<spdlog::logger> logger)
    : d(new internal::PluginManagerPrivate(logger)) {}

PluginManager::~PluginManager() {}

void PluginManager::addSearchPath(const std::string &path) {
  if (d->search_paths.find(path) != d->search_paths.end()) {
  }
  d->search_paths.insert(path);
}

void PluginManager::loadPlugins() {
  d->readPlugins();
  d->loadPlugins();
}

void PluginManager::registerProvider(IPluginProvider *provider) {
  d->providers.push_back(provider);
}

/*
void PluginManager::loadPlugin(const std::string &path) {}

void PluginManager::loadPlugin(PluginSpec *spec, PluginSpec::State state) {}*/

PLUGINS_NS_END

#include "plugins/plugin-manager.hpp"
#include "helpers.hpp"
#include "plugins/iplugin-provider.hpp"
#include <iostream>
#include <plugins/pluginspec.hpp>
#include <set>
#include <tinydir.h>
#include <vector>

PLUGINS_NS_BEGIN

namespace internal {
class PluginManagerPrivate {
public:
  PluginManagerPrivate() {}

  std::set<std::string> search_paths;
  std::vector<IPluginProvider *> providers;
};
} // namespace internal

PluginManager::PluginManager() : d(new internal::PluginManagerPrivate) {}

PluginManager::~PluginManager() {}

void PluginManager::addSearchPath(const std::string &path) {
  d->search_paths.insert(path);
}

void PluginManager::loadPlugins() {

  for (auto i : d->search_paths) {
    tinydir_dir dir;
    tinydir_open(&dir, i.c_str());
    normalizePath(i);

    while (dir.has_next) {
      tinydir_file file;
      tinydir_readfile(&dir, &file);

      if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0) {
        tinydir_next(&dir);
        continue;
      }

      loadPlugin(i + "/" + file.name);

      tinydir_next(&dir);
    }

    tinydir_close(&dir);
  }
}

void PluginManager::registerProvider(IPluginProvider *provider) {
  d->providers.push_back(provider);
}

void PluginManager::loadPlugin(const std::string &path) {

  for (auto p : d->providers) {
    auto spec = p->load(path);
    if (spec) {
      std::cout << "pliugin " << spec->name() << std::endl;
    }
  }
}

PLUGINS_NS_END

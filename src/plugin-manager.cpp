#include "plugins/plugin-manager.hpp"
#include "helpers.hpp"
#include "plugins/iplugin-provider.hpp"
#include "pluginspec_p.hpp"
#include <iostream>
#include <plugins/pluginspec.hpp>
#include <set>
#include <spdlog/spdlog.h>
#include <tinydir.h>
#include <vector>

PLUGINS_NS_BEGIN

namespace internal {
class PluginManagerPrivate {
public:
  PluginManagerPrivate() {}

  void loadPlugin(PluginSpec *spec, PluginSpec::State state);

  std::set<std::string> search_paths;
  std::vector<IPluginProvider *> providers;
  std::vector<PluginSpec *> specs;
  std::shared_ptr<spdlog::logger> logger;

private:
  bool loadQueue(PluginSpec *spec, std::vector<PluginSpec *> &queue,
                 std::vector<PluginSpec *> &circularityCheckQueue);
  std::vector<PluginSpec *> loadQueue();
};

void PluginManagerPrivate::loadPlugin(PluginSpec *spec,
                                      PluginSpec::State state) {}

bool PluginManagerPrivate::loadQueue(
    PluginSpec *spec, std::vector<PluginSpec *> &queue,
    std::vector<PluginSpec *> &circularityCheckQueue) {
  if (std::find(queue.begin(), queue.end(), spec) != queue.end()) {
    return true;
  }

  if (std::find(circularityCheckQueue.begin(), circularityCheckQueue.end(),
                spec)) {
    // spec->d->has_error = true;
    spec->d->error = PluginSpec::CircularDependency;
    // FIXME: set dependency for use in error management somehow

    return false;
  }

  circularityCheckQueue.push_back(spec);
  // check if we have the dependencies
  if (spec->state() == PluginSpec::Invalid ||
      spec->state() == PluginSpec::Read) {
    queue.pus_pack(spec);
  }

  std::vector<PluginSpec *> PluginManagerPrivate::loadQueue() {}

} // namespace internal

PluginManager::PluginManager() : d(new internal::PluginManagerPrivate) {}

PluginManager::~PluginManager() {}

void PluginManager::addSearchPath(const std::string &path) {
  if (d->search_paths.find(path) != d->search_paths.end()) {
  }
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
      auto meta = spec->metadata();
      if (spec->d->read(meta)) {
        std::cout << "plugin " << spec->name() << std::endl;
      } else {
        std::cout << "error " << spec->d->error_string << std::endl;
      }
    }
  }
}

void PluginManager::loadPlugin(PluginSpec *spec, PluginSpec::State state) {}

PLUGINS_NS_END

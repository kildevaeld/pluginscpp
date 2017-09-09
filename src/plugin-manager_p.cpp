#include "plugin-manager_p.hpp"
#include <plugins/pluginspec.hpp>
PLUGINS_NS_BEGIN

namespace internal {

void PluginManagerPrivate::loadPlugins() {
  auto queue = loadQueue();
  for (auto spec : queue) {
    loadPlugin(spec, PluginSpec::Loaded);
  }
}

void PluginManagerPrivate::loadPlugin(PluginSpec *spec,
                                      PluginSpec::State destState) {
  if (spec->has_error() || spec->state() != destState - 1) {
    if (spec->state() != destState - 1) {
      logger->error("plugin '{0}@{1}' invalid spec state {2} => {3}",
                    spec->name(), spec->version(), spec->state(), destState);
    }
    return;
  }
}

bool PluginManagerPrivate::loadQueue(
    PluginSpec *spec, std::vector<PluginSpec *> &queue,
    std::vector<PluginSpec *> &circularityCheckQueue) {
  if (std::find(queue.begin(), queue.end(), spec) != queue.end()) {
    return true;
  }

  if (std::find(circularityCheckQueue.begin(), circularityCheckQueue.end(),
                spec) != circularityCheckQueue.end()) {
    // spec->d->has_error = true;
    spec->d->error = PluginSpec::CircularDependency;
    // FIXME: set dependency for use in error management somehow

    return false;
  }

  circularityCheckQueue.push_back(spec);
  // check if we have the dependencies
  if (spec->state() == PluginSpec::Invalid ||
      spec->state() == PluginSpec::Read) {
    queue.push_back(spec);
    return false;
  }

  queue.push_back(spec);
  return true;
}

std::vector<PluginSpec *> PluginManagerPrivate::loadQueue() {
  std::vector<PluginSpec *> queue;
  for (auto spec : specs) {
    std::vector<PluginSpec *> circularityCheckQueue;
    loadQueue(spec, queue, circularityCheckQueue);
  }
  return queue;
}

void PluginManagerPrivate::readPlugin(const std::string &path) {
  for (auto p : providers) {
    auto spec = p->load(path);
    if (spec) {
      std::cout << "pliugin " << spec->name() << std::endl;
      auto meta = spec->metadata();
      if (spec->d->read(meta)) {
        specs.push_back(spec);
      } else {
        std::cout << "error " << spec->d->error << std::endl;
      }
    }
  }
}

void PluginManagerPrivate::readPlugins() {
  for (auto i : search_paths) {
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

      readPlugin(i + "/" + file.name);

      tinydir_next(&dir);
    }

    tinydir_close(&dir);
  }
}

void PluginManagerPrivate::resolveDependencies() {
  foreach (PluginSpec *spec, pluginSpecs) {
    spec->d->enabledIndirectly = false; // reset, is recalculated below
    spec->d->resolveDependencies(pluginSpecs);
  }

  Utils::reverseForeach(loadQueue(), [](PluginSpec *spec) {
    spec->d->enableDependenciesIndirectly();
  });
}

} // namespace internal

PLUGINS_NS_END

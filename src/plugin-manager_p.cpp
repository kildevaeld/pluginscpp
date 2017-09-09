#include "plugin-manager_p.hpp"
#include "helpers.hpp"
#include "pluginspec_p.hpp"
#include <iostream>
#include <plugins/iplugin-provider.hpp>
#include <plugins/pluginspec.hpp>
#include <tinydir.h>
#include <utils/algorithms.hpp>

PLUGINS_NS_BEGIN

namespace internal {

void PluginManagerPrivate::loadPlugins() {
  auto queue = loadQueue();
  for (auto spec : queue) {
    loadPlugin(spec, PluginSpec::Loaded);
  }
}

void PluginManagerPrivate::initializePlugins() {
  auto queue = loadQueue();
  // Utils::setMimeStartupPhase(MimeStartupPhase::PluginsInitializing);
  for (auto spec : queue) {
    loadPlugin(spec, PluginSpec::Initialized);
  }

  // Utils::setMimeStartupPhase(MimeStartupPhase::PluginsDelayedInitializing);
  utils::algorithms::reverseForeach(queue, [this](PluginSpec *spec) {
    loadPlugin(spec, PluginSpec::Running);
    if (spec->state() == PluginSpec::Running) {
      // delayedInitializeQueue.append(spec);
    } else {
      // Plugin initialization failed, so cleanup after it
      spec->d->kill();
    }
  });

  // emit q->pluginsChanged();
  // Utils::setMimeStartupPhase(MimeStartupPhase::UpAndRunning);

  // delayedInitializeTimer = new QTimer;
  // delayedInitializeTimer->setInterval(100);
  // delayedInitializeTimer->setSingleShot(true);
  // connect(delayedInitializeTimer, &QTimer::timeout, this,
  //        &PluginManagerPrivate::nextDelayedInitialize);
  // delayedInitializeTimer->start();
}

void PluginManagerPrivate::loadPlugin(PluginSpec *spec,
                                      PluginSpec::State destState) {
  if (spec->has_error() || spec->state() != destState - 1) {
    if (spec->state() != destState - 1) {
      logger->error("plugin '{0}@{1}' invalid spec state {2} => {3}",
                    spec->name(), spec->version(), spec->state(), destState);
    }
  }

  if (!spec->isEffectivelyEnabled() && destState == PluginSpec::Loaded)
    return;

  switch (destState) {
  case PluginSpec::Running:
    profilingReport(">initializeExtensions", spec);
    spec->d->initializeExtensions();
    profilingReport("<initializeExtensions", spec);
    return;
  case PluginSpec::Deleted:
    profilingReport(">delete", spec);
    spec->d->kill();
    profilingReport("<delete", spec);
    return;
  default:
    break;
  }

  for (auto it : spec->dependencySpecs()) {
  }
  // check if dependencies have loaded without error
  /*QHashIterator<PluginDependency, PluginSpec *> it(spec->dependencySpecs());
  while (it.hasNext()) {
    it.next();
    if (it.key().type != PluginDependency::Required)
      continue;
    PluginSpec *depSpec = it.value();
    if (depSpec->state() != destState) {
      spec->d->hasError = true;
      spec->d->errorString =
          PluginManager::tr("Cannot load plugin because dependency failed to "
                            "load: %1(%2)\nReason: %3")
              .arg(depSpec->name())
              .arg(depSpec->version())
              .arg(depSpec->errorString());
      return;
    }
  }*/
  switch (destState) {
  case PluginSpec::Loaded:
    profilingReport(">loadLibrary", spec);
    spec->d->loadLibrary();
    profilingReport("<loadLibrary", spec);
    break;
  case PluginSpec::Initialized:
    profilingReport(">initializePlugin", spec);
    spec->d->initializePlugin();
    profilingReport("<initializePlugin", spec);
    break;
  case PluginSpec::Stopped:
    profilingReport(">stop", spec);
    if (spec->d->stop() == IPlugin::AsynchronousShutdown) {
      // asynchronousPlugins << spec;
      // connect(spec->plugin(), &IPlugin::asynchronousShutdownFinished, this,
      //        &PluginManagerPrivate::asyncShutdownFinished);
    }
    profilingReport("<stop", spec);
    break;
  default:
    break;
  }
}

void PluginManagerPrivate::profilingReport(const char *what,
                                           const PluginSpec *spec) {
  /*if (!m_profileTimer.isNull()) {
    const int absoluteElapsedMS = m_profileTimer->elapsed();
    const int elapsedMS = absoluteElapsedMS - m_profileElapsedMS;
    m_profileElapsedMS = absoluteElapsedMS;
    if (spec)
      m_profileTotal[spec] += elapsedMS;
    if (spec)
      qDebug("%-22s %-22s %-22s %8dms (%8dms)", what, qPrintable(spec->name()),
             qPrintable(spec->iid()), absoluteElapsedMS, elapsedMS);
    else
      qDebug("%-45s %8dms (%8dms)", what, absoluteElapsedMS, elapsedMS);
  }*/
  std::cout << what << " " << spec->name() << std::endl;
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

  resolveDependencies();

  utils::algorithms::sort(specs, &PluginSpec::name);
}

void PluginManagerPrivate::resolveDependencies() {

  for (auto spec : specs) {
    spec->d->enabledIndirectly = false;
    spec->d->resolveDependencies(specs);
  }
  utils::algorithms::reverseForeach(loadQueue(), [](PluginSpec *spec) {
    spec->d->enableDependenciesIndirectly();
  });
  /*foreach (PluginSpec *spec, pluginSpecs) {
    spec->d->enabledIndirectly = false; // reset, is recalculated below
    spec->d->resolveDependencies(pluginSpecs);
  }

  Utils::reverseForeach(loadQueue(), [](PluginSpec *spec) {
    spec->d->enableDependenciesIndirectly();
  });*/
}

void PluginManagerPrivate::shutdown() {
  stopAll();
  /*if (!asynchronousPlugins.isEmpty()) {
    shutdownEventLoop = new QEventLoop;
    shutdownEventLoop->exec();
  }*/
  deleteAll();
  /*if (!m_allObjects.isEmpty()) {
    qDebug() << "There are" << m_allObjects.size()
             << "objects left in the plugin manager pool.";
    // Intentionally split debug info here, since in case the list contains
    // already deleted object we get at least the info about the number of
    // objects;
    qDebug() << "The following objects left in the plugin manager pool:"
             << m_allObjects;
  }*/
}

void PluginManagerPrivate::stopAll() {
  /*if (delayedInitializeTimer && delayedInitializeTimer->isActive()) {
    delayedInitializeTimer->stop();
    delete delayedInitializeTimer;
    delayedInitializeTimer = 0;
  }
  QList<PluginSpec *> queue = loadQueue();
  foreach (PluginSpec *spec, queue) { loadPlugin(spec, PluginSpec::Stopped); }*/
  auto queue = loadQueue();
  for (auto spec : queue) {
    loadPlugin(spec, PluginSpec::Stopped);
  }
}

void PluginManagerPrivate::deleteAll() {
  utils::algorithms::reverseForeach(loadQueue(), [this](PluginSpec *spec) {
    loadPlugin(spec, PluginSpec::Deleted);
  });
}

} // namespace internal

PLUGINS_NS_END

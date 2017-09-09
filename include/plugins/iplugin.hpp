#pragma once
#include "defines.hpp"
#include <string>

PLUGINS_NS_BEGIN

class PluginSpec;

namespace internal {
class IPluginPrivate;
}

class IPlugin {

public:
  enum ShutdownFlag { SynchronousShutdown, AsynchronousShutdown };
  IPlugin();
  virtual ~IPlugin();
  virtual void loaded() {}
  virtual bool initialize(const std::vector<std::string> &args,
                          std::string *err) = 0;
  virtual void extensionsInitialized() {}
  virtual bool delayedInitialize() { return true; }
  virtual ShutdownFlag aboutToShutdown() { return SynchronousShutdown; }

  PluginSpec *spec() const;

private:
  std::unique_ptr<internal::IPluginPrivate> d;
};

PLUGINS_NS_END

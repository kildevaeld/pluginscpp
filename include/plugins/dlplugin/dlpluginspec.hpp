#pragma once
#include <plugins/defines.hpp>
#include <plugins/dlplugin/dlpluginloader.hpp>
#include <plugins/pluginspec.hpp>
PLUGINS_NS_BEGIN

namespace internal {
class DLPluginSpecPrivate;
}

class DLPluginSpec : public PluginSpec {

private:
  void readMetadata();
  DLPluginSpec(PluginLoader &&loader);

  // PluginSpec interface
public:
  virtual IPlugin *plugin();

private:
  friend class DLPluginManager;
  std::unique_ptr<internal::DLPluginSpecPrivate> d;
};

PLUGINS_NS_END

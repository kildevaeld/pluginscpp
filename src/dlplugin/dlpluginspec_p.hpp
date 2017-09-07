#pragma once
#include <plugins/defines.hpp>
#include <plugins/dlplugin/dlpluginloader.hpp>
#include <plugins/dlplugin/dlpluginspec.hpp>

PLUGINS_NS_BEGIN

namespace internal {

class DLPluginSpecPrivate {
public:
  DLPluginSpecPrivate(PluginLoader &&l);

  PluginLoader loader;
};
}

PLUGINS_NS_END

#pragma once
#include <plugins/defines.hpp>
#include <plugins/dlplugin/dlpluginloader.hpp>
#include <plugins/dlplugin/dlpluginspec.hpp>

PLUGINS_NS_BEGIN

namespace internal {

class DLPluginSpecPrivate {
public:
  DLPluginSpecPrivate(PluginLoader &&l) : loader(std::move(l)) {}

  PluginLoader loader;
};
} // namespace internal

PLUGINS_NS_END

#include "pluginspec_p.hpp"
#include <iostream>
#include <plugins/dlplugin/dlpluginspec.hpp>

PLUGINS_NS_BEGIN

DLPluginSpec::DLPluginSpec(PluginLoader &&loader)
    : d(new internal::PluginSpecPrivate(std::move(loader))) {}

IPlugin *DLPluginSpec::plugin() { return d->loader.load(); }

PLUGINS_NS_END

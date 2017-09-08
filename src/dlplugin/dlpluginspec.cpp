#include "dlpluginspec_p.hpp"
#include <iostream>
#include <plugins/dlplugin/dlpluginspec.hpp>

PLUGINS_NS_BEGIN

DLPluginSpec::DLPluginSpec(PluginLoader &&loader)
    : d(new internal::DLPluginSpecPrivate(std::move(loader))) {}

IPlugin *DLPluginSpec::plugin() { return d->loader.load(); }

PLUGINS_NS_END

json plugins::DLPluginSpec::metadata() const { return d->loader.metadata(); }

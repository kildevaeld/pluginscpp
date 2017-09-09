#include <plugins/iplugin.hpp>
#include <plugins/pluginspec.hpp>

PLUGINS_NS_BEGIN

namespace internal {

class IPluginPrivate {
public:
  IPluginPrivate() {}

  PluginSpec *q;
};

} // namespace internal

IPlugin::IPlugin() {}

IPlugin::~IPlugin() {}

PluginSpec *IPlugin::spec() const { return d->q; }

// namespace internal

PLUGINS_NS_END

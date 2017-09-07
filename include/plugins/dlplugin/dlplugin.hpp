#pragma once
#include <plugins/defines.hpp>
#include <plugins/iplugin-provider.hpp>
#include <plugins/iplugin.hpp>

PLUGINS_NS_BEGIN

class PluginSpec;

class DLPluginManager : public IPluginProvider {
  // IPluginProvider interface
public:
  virtual PluginSpec *load(const std::string &path);
};

PLUGINS_NS_END

#define DEFINE_PLUGIN(classType, pluginName, pluginVersion)                    \
  extern const unsigned char plugin_metadata_json[];                           \
  extern unsigned int plugin_metadata_json_len;                                \
  extern "C" {                                                                 \
  std::unique_ptr<plugins::IPlugin> load() {                                   \
    return std::make_unique<classType>();                                      \
  }                                                                            \
                                                                               \
  const char *metadata() { return (char *)plugin_metadata_json; }              \
  }

#pragma once
#include <json.hpp>
#include <memory>
#include <plugins/defines.hpp>
#include <plugins/iplugin.hpp>
#include <string>
using json = nlohmann::json;

PLUGINS_NS_BEGIN

namespace internal {
class PluginLoaderPrivate;
}

class PluginLoader {
public:
  PluginLoader() = delete;
  PluginLoader(const PluginLoader &other) = delete;
  PluginLoader(PluginLoader &&other);
  PluginLoader(std::string name);
  ~PluginLoader();

  /*std::string get_name();
  std::string get_version();*/
  json metadata() const;
  IPlugin *load();
  bool is_valid() const;

private:
  std::unique_ptr<internal::PluginLoaderPrivate> d;
};

PLUGINS_NS_END

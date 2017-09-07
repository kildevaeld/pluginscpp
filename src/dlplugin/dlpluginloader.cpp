#include <dlfcn.h>
#include <iostream>
#include <plugins/dlplugin/dlpluginloader.hpp>
using json = nlohmann::json;

PLUGINS_NS_BEGIN

namespace internal {

class PluginLoaderPrivate {
public:
  PluginLoaderPrivate() {}

  std::unique_ptr<IPlugin> (*_load)();
  void *handle;
  char *(*_get_name)();
  char *(*_get_version)();
  char *(*_get_metadata)();
  bool m_valid = false;
  std::unique_ptr<IPlugin> instance = nullptr;
};

} // namespace internal

PluginLoader::PluginLoader(PluginLoader &&other) { d.swap(other.d); }

PluginLoader::PluginLoader(std::string name)
    : d(new internal::PluginLoaderPrivate) {
  d->handle = dlopen(name.c_str(), RTLD_LAZY);
  if (!d->handle) {
    d->m_valid = false;
    return;
  }
  d->_load = (std::unique_ptr<IPlugin>(*)())dlsym(d->handle, "load");
  // d->_get_name = (char *(*)())dlsym(d->handle, "name");
  // d->_get_version = (char *(*)())dlsym(d->handle, "version");
  d->_get_metadata = (char *(*)())dlsym(d->handle, "metadata");
  /*if (d->_get_name && d->_get_version && d->_get_metadata) {
    if (strlen(d->_get_name()) > 0 && strlen(d->_get_version()) > 0) {
      d->m_valid = true;
    }
  }*/
  if (d->_get_metadata) {
    d->m_valid = true;
  }
}

PluginLoader::~PluginLoader() {}
/*
std::string PluginLoader::get_name() { return std::string(d->_get_name()); }

std::string PluginLoader::get_version() {
  return std::string(d->_get_version());
}*/

json PluginLoader::metadata() const {
  json out(d->_get_metadata());
  return out;
}

IPlugin *PluginLoader::load() {
  if (!d->instance)
    d->instance = d->_load();
  return d->instance.get();
}

bool PluginLoader::is_valid() const {
  return d->m_valid && (d->_load && d->_get_metadata);
}

PLUGINS_NS_END

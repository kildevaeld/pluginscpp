#pragma once
#include "defines.hpp"
#include <string>
#include "pluginspec.hpp"

PLUGINS_NS_BEGIN


class IPluginProvider {

public:
  IPluginProvider();
  virtual ~IPluginProvider();

  virtual PluginSpec *load(const std::string &path) = 0;
};

PLUGINS_NS_END

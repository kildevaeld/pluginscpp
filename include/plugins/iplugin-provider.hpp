#pragma once
#include "defines.hpp"
#include <string>

PLUGINS_NS_BEGIN

class PluginSpec;

class IPluginProvider {

public:
  IPluginProvider();
  virtual ~IPluginProvider();

  virtual PluginSpec *load(const std::string &path) = 0;
};

PLUGINS_NS_END

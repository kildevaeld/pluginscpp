#pragma once
#include "defines.hpp"
#include <json.hpp>
#include <memory>
#include <string>

using json = nlohmann::json;

PLUGINS_NS_BEGIN

class IPlugin;

namespace internal {
class PluginSpecPrivate;
}

struct PluginDependency {
  enum Type { Required, Optional, Test };

  PluginDependency() : type(Required) {}

  std::string name;
  std::string version;
  Type type;
  bool operator==(const PluginDependency &other) const;
  std::string toString() const;
};

class PluginSpec {

public:
  enum State {
    Invalid,
    Read,
    Resolved,
    Loaded,
    Initialized,
    Running,
    Stopped,
    Deleted
  };

  enum Error { Valid, CircularDependency };

  PluginSpec();
  ~PluginSpec();

  std::string name() const;
  std::string version() const;

  std::vector<PluginDependency> dependencies() const;

  virtual IPlugin *plugin() = 0;
  virtual json metadata() const = 0;

private:
  std::unique_ptr<internal::PluginSpecPrivate> d;
  friend class plugins::PluginManager;
};

PLUGINS_NS_END

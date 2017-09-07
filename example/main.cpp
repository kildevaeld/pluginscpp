#include <iostream>
#include <plugins/dlplugin/dlplugin.hpp>
#include <plugins/plugin-manager.hpp>

using plugins::PluginManager;

int main(int argc, char **argv) {
  PluginManager manager;

  manager.registerProvider(new plugins::DLPluginManager());

  manager.addSearchPath("./example");
  manager.loadPlugins();
}

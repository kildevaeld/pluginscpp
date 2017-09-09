#include <iostream>
#include <plugins/dlplugin/dlplugin.hpp>
#include <plugins/plugin-manager.hpp>
#include <spdlog/spdlog.h>
using plugins::PluginManager;

int main(int argc, char **argv) {
  PluginManager manager(spdlog::stdout_color_st("pl"));

  manager.registerProvider(new plugins::DLPluginManager());

  manager.addSearchPath("./example");
  manager.loadPlugins();
}

#include <plugins/dlplugin/dlplugin.hpp>
#include <plugins/iplugin.hpp>

class TestPlugin : public plugins::IPlugin {};

DEFINE_PLUGIN(TestPlugin, "Test plugin", "0.0.1")

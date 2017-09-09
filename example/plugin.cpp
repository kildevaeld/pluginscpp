#include <plugins/dlplugin/dlplugin.hpp>
#include <plugins/iplugin.hpp>

class TestPlugin : public plugins::IPlugin {

public:
    virtual bool initialize(const std::vector<std::string> &args, std::string *err) {
        return true;
    }

};

DEFINE_PLUGIN(TestPlugin, "Test plugin", "0.0.1")

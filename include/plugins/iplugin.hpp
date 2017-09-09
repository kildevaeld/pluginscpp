#pragma once
#include "defines.hpp"
#include <string>

PLUGINS_NS_BEGIN

class IPlugin {

public:
	IPlugin() {}
	virtual ~IPlugin() {}

	virtual bool initialize(const std::vector<std::string> &args, std::string *err) = 0;
	virtual void extensionsInitialized() {}

};


PLUGINS_NS_END
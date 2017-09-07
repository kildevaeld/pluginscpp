#include "helpers.hpp"

PLUGINS_NS_BEGIN

void normalizePath(std::string &path) {
  if (path[path.length() - 1] == '/' && path != "/") {
    path.pop_back();
  } else if (path == "") {
    path.push_back('/');
  }
}

PLUGINS_NS_END

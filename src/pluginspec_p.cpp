#include "pluginspec_p.hpp"
#include <fmt/format.h>
#include <map>
#include <plugins/pluginspec.hpp>
#include <sstream>
#include <utils/algorithms.hpp>
#include <plugins/iplugin.hpp>

PLUGINS_NS_BEGIN

namespace internal {

namespace {
const char PLUGIN_METADATA[] = "MetaData";
const char PLUGIN_NAME[] = "name";
const char PLUGIN_VERSION[] = "version";
const char PLUGIN_COMPATVERSION[] = "CompatVersion";
const char PLUGIN_REQUIRED[] = "Required";
const char PLUGIN_HIDDEN_BY_DEFAULT[] = "HiddenByDefault";
const char PLUGIN_EXPERIMENTAL[] = "Experimental";
const char PLUGIN_DISABLED_BY_DEFAULT[] = "DisabledByDefault";
const char VENDOR[] = "Vendor";
const char COPYRIGHT[] = "Copyright";
const char LICENSE[] = "License";
const char DESCRIPTION[] = "Description";
const char URL[] = "Url";
const char CATEGORY[] = "Category";
const char PLATFORM[] = "Platform";
const char DEPENDENCIES[] = "dependencies";
const char DEPENDENCY_NAME[] = "name";
const char DEPENDENCY_VERSION[] = "version";
const char DEPENDENCY_IID[] = "Context";
const char DEPENDENCY_TYPE[] = "type";
const char DEPENDENCY_TYPE_SOFT[] = "optional";
const char DEPENDENCY_TYPE_HARD[] = "required";
const char DEPENDENCY_TYPE_TEST[] = "test";
const char ARGUMENTS[] = "Arguments";
const char ARGUMENT_NAME[] = "Name";
const char ARGUMENT_PARAMETER[] = "Parameter";
const char ARGUMENT_DESCRIPTION[] = "Description";
} // namespace

bool PluginSpecPrivate::report_error(const std::string &msg) {
  // error_string = msg;
  has_error = true;
  return false;
}

static inline std::string msgValueMissing(const char *key) {
  std::stringstream ss;
  ss << "\"" << key << "\" is missing";
  return ss.str();
}

PluginSpecPrivate::PluginSpecPrivate(plugins::PluginSpec *spec) : q(spec) {}

bool PluginSpecPrivate::read(const json &metadata) {
  auto m = metadata;

  if (!m.is_object()) {
    return report_error("not a object");
  }

  auto n = m.find(PLUGIN_NAME);
  if (n == m.end()) {
    return report_error("name is missing");
  }

  name = (*n).get<std::string>();

  n = m.find(PLUGIN_VERSION);
  if (n == m.end()) {
    return report_error("version is missing");
  }

  version = (*n).get<std::string>();

  n = m.find(DEPENDENCIES);

  if (n != m.end() && n->is_array()) {
    PluginDependency dependency;
    for (auto &dep : *n) {
      auto it = dep.find(DEPENDENCY_NAME);
      if (it == dep.end()) {
        return report_error(msgValueMissing(DEPENDENCY_NAME));
      }
      dependency.name = it->get<std::string>();
      it = dep.find(DEPENDENCY_VERSION);
      if (it == dep.end()) {
        return report_error(msgValueMissing(DEPENDENCY_VERSION));
      }
      dependency.version = it->get<std::string>();

      dependencies.push_back(std::move(dependency));
    }
  }

  state = PluginSpec::Read;

  return true;
}

bool PluginSpecPrivate::resolveDependencies(
    const std::vector<PluginSpec *> &specs) {
  
  if (has_error)
    return false;
  if (state == PluginSpec::Resolved)
    state =
        PluginSpec::Read; // Go back, so we just re-resolve the dependencies.
  if (state != PluginSpec::Read) {
    error_string = "Resolving dependencies failed because state != Read";
    error = PluginSpec::DependencyInvalidState;
    has_error = true;
    return false;
  }
  std::map<PluginDependency, PluginSpec *> resolvedDependencies;
  for (auto &dependency : dependencies) {
    PluginSpec *const found = utils::algorithms::findOrDefault(
        specs, [&dependency](PluginSpec *spec) {
          return spec->provides(dependency.name, dependency.version);
        });
    if (!found) {
      if (dependency.type == PluginDependency::Required) {
        has_error = true;
       
        if (error_string.length() > 0) {
          error_string.append("\n");
        }
        error_string.append(
            fmt::format("could not resolve dependency '{0}({1})'",
                        dependency.name, dependency.version));
      }
      continue;
    }

     //resolvedDependencies.insert(dependency, found);
  }
  if (has_error)
    return false;

  dependencySpecs = resolvedDependencies;
  state = PluginSpec::Resolved;
}

bool PluginSpecPrivate::loadLibrary() {
  if (has_error)
    return false;
  if (state != PluginSpec::Resolved) {
    if (state == PluginSpec::Loaded)
      return true;
    has_error = true;
    error_string = "Loading the library failed because state != Resolved";
    return false;
  }

  if (!q->plugin()) {
    has_error = true;
    error_string = "could not load";
    return false;
  }

  state = PluginSpec::Loaded;

  /*if (!loader.load()) {
    hasError = true;
    errorString = QDir::toNativeSeparators(filePath) +
                  QString::fromLatin1(": ") + loader.errorString();
    return false;
  }*/
  /*IPlugin *pluginObject = qobject_cast<IPlugin *>(loader.instance());
  if (!pluginObject) {
    hasError = true;
    errorString = QCoreApplication::translate(
        "PluginSpec", "Plugin is not valid (does not derive from IPlugin)");
    loader.unload();
    return false;
  }
  state = PluginSpec::Loaded;
  plugin = pluginObject;
  plugin->d->pluginSpec = q;

  pluginObject->loaded();*/

  return true;
}


bool PluginSpecPrivate::initializePlugin() {
  if (has_error)
    return false;
  if (state != PluginSpec::Loaded) {
    if (state == PluginSpec::Initialized)
      return true;    
    error_string = "Initializing the plugin failed because state != Loaded";
    has_error = true;
    return false;
  }
  if (!q->plugin()) {
    error_string = "Internal error: have no plugin instance to initialize";
    has_error = true;
    return false;
  }
  std::string err;
  if (!q->plugin()->initialize(arguments, &err)) {
    error_string = fmt::format("Plugin initialization failed: {0}", err);
    has_error = true;
    return false;
  }
  state = PluginSpec::Initialized;
  return true;
}

bool PluginSpecPrivate::initializeExtensions() {
  if (has_error)
    return false;
  if (state != PluginSpec::Initialized) {
    if (state == PluginSpec::Running)
      return true;
    error_string = "Cannot perform extensionsInitialized because state != Initialized";
    has_error = true;
    return false;
  }
  if (!q->plugin()) {
    error_string =  "Internal error: have no plugin instance to perform extensionsInitialized";
    has_error = true;
    return false;
  }
  q->plugin()->extensionsInitialized();
  state = PluginSpec::Running;
  return true;
}

void PluginSpecPrivate::kill() {

}

void PluginSpecPrivate::enableDependenciesIndirectly() {
  if (!q->isEffectivelyEnabled()) // plugin not enabled, nothing to do
    return;
 
  for (auto it: dependencySpecs) {
    if (it.first.type != PluginDependency::Required)
      continue;
     PluginSpec *dependencySpec = it.second;
     //if (!dependencySpec->isEffectivelyEnabled()) 
       //dependencySpec->d->enabledIndirectly = true;
  }
}

/*
static inline QString msgValueMissing(const char *key) {
  return QCoreApplication::translate("PluginSpec", "\"%1\" is missing")
      .arg(QLatin1String(key));
}

static inline QString msgValueIsNotAString(const char *key) {
  return QCoreApplication::translate("PluginSpec",
                                     "Value for key \"%1\" is not a string")
      .arg(QLatin1String(key));
}

static inline QString msgValueIsNotABool(const char *key) {
  return QCoreApplication::translate("PluginSpec",
                                     "Value for key \"%1\" is not a bool")
      .arg(QLatin1String(key));
}

static inline QString msgValueIsNotAObjectArray(const char *key) {
  return QCoreApplication::translate(
             "PluginSpec", "Value for key \"%1\" is not an array of objects")
      .arg(QLatin1String(key));
}

static inline QString msgValueIsNotAMultilineString(const char *key) {
  return QCoreApplication::translate(
             "PluginSpec",
             "Value for key \"%1\" is not a string and not an array of strings")
      .arg(QLatin1String(key));
}

static inline QString msgInvalidFormat(const char *key,
                                       const QString &content) {
  return QCoreApplication::translate(
             "PluginSpec", "Value \"%2\" for key \"%1\" has invalid format")
      .arg(QLatin1String(key), content);
}*/

} // namespace internal

PLUGINS_NS_END

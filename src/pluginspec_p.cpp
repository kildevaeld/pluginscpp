#include "pluginspec_p.hpp"
#include <fmt/format.h>
#include <map>
#include <plugins/pluginspec.hpp>
#include <sstream>
#include <utils/algorithms.hpp>

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
        /*if (!errorString.isEmpty())
          errorString.append(QLatin1Char('\n'));
        errorString.append(
            QCoreApplication::translate("PluginSpec",
                                        "Could not resolve dependency '%1(%2)'")
                .arg(dependency.name)
                .arg(dependency.version));*/
        if (error_string.length() > 0) {
          error_string.append("\n");
        }
        error_string.append(
            fmt::format("could not resolve dependency '{0}({1}})'",
                        dependency.name, dependency.version));
      }
      continue;
    }

    // resolvedDependencies.insert(dependency, found);
  }
  if (has_error)
    return false;

  dependencySpecs = resolvedDependencies;

  state = PluginSpec::Resolved;
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

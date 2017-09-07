#include "pluginspec_p.hpp"

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
const char DEPENDENCIES[] = "Dependencies";
const char DEPENDENCY_NAME[] = "Name";
const char DEPENDENCY_VERSION[] = "Version";
const char DEPENDENCY_IID[] = "Context";
const char DEPENDENCY_TYPE[] = "Type";
const char DEPENDENCY_TYPE_SOFT[] = "optional";
const char DEPENDENCY_TYPE_HARD[] = "required";
const char DEPENDENCY_TYPE_TEST[] = "test";
const char ARGUMENTS[] = "Arguments";
const char ARGUMENT_NAME[] = "Name";
const char ARGUMENT_PARAMETER[] = "Parameter";
const char ARGUMENT_DESCRIPTION[] = "Description";
} // namespace

PluginSpecPrivate::PluginSpecPrivate(plugins::PluginSpec *spec)
    : loader(std::move(spec)) {}

bool PluginSpecPrivate::read_metadata(const json &metadata) {

  if (!metadata.is_object()) {
    return false;
  }

  auto n = metadata.find(PLUGIN_NAME);
  if (n == metadata.end()) {
    return report_error("name is missing");
  }

  name = (*n).get<std::string>();

  n = metadata.find(PLUGIN_VERSION);
  if (n == metadata.end()) {
    return report_error("version is missing");
  }

  version = (*n).get<std::string>();

  return true;
}

bool PluginSpecPrivate::report_error(const std::string &msg) {
  error_string = msg;
  has_error = true;
  return false;
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

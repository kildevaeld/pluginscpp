
set(SOURCES
  iplugin.cpp
  plugin-manager.cpp
  plugin-manager_p.cpp
  pluginspec.cpp
  pluginspec_p.cpp
  iplugin-provider.cpp
  helpers.cpp
)

add_library(plugins ${SOURCES})
target_include_directories(plugins PUBLIC ${PROJECT_ROOT}/include)

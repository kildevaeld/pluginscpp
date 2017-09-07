

set(SOURCES
    dlplugin.cpp
    dlpluginspec.cpp
    dlpluginloader.cpp
    pluginspec_p.cpp
)

add_library(dlplugin ${SOURCES})
target_link_libraries(dlplugin dl plugins)
target_include_directories(dlplugin PUBLIC ${PROJECT_ROOT}/include)

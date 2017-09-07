

add_executable(pl main.cpp)
target_link_libraries(pl plugins dlplugin)

add_resources(JSON plugin.json)


add_library(testplugin SHARED plugin.cpp ${JSON})
target_link_libraries(testplugin plugins dlplugin)

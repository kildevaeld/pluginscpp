set(CURRENT ${CMAKE_CURRENT_LIST_DIR})

# Compilation step for static resoures
FUNCTION(ADD_RESOURCES out_var)
SET(result)
FOREACH(in_f ${ARGN})
  message(info ${CURRENT})
  FILE(RELATIVE_PATH src_f ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${in_f})
  SET(out_f "${PROJECT_BINARY_DIR}/${in_f}.c")
  ADD_CUSTOM_COMMAND(OUTPUT ${out_f}
    COMMAND node ${CURRENT}/generate ${src_f} ${out_f}
    DEPENDS ${in_f}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Building GLSL object ${out_f}"
    VERBATIM
    )
  LIST(APPEND result ${out_f})
ENDFOREACH()
SET(${out_var} "${result}" PARENT_SCOPE)
ENDFUNCTION()
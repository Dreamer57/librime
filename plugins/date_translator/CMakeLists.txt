project(rime-date_translator)
cmake_minimum_required(VERSION 3.10)

aux_source_directory(src date_translator_src)

add_library(rime-date_translator-objs OBJECT ${date_translator_src})
if(BUILD_SHARED_LIBS)
  set_target_properties(rime-date_translator-objs
    PROPERTIES
    POSITION_INDEPENDENT_CODE ON)
endif()

set(date_translator_library rime-date_translator)
set(plugin_name ${date_translator_library} PARENT_SCOPE)
set(plugin_objs $<TARGET_OBJECTS:rime-date_translator-objs> PARENT_SCOPE)
set(date_translator_deps
  ${rime_library}
  ${rime_gears_library}
)
set(plugin_deps ${date_translator_deps} PARENT_SCOPE)
set(plugin_modules "date_translator" PARENT_SCOPE)
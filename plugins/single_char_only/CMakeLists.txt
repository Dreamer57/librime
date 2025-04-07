project(rime-single_char_only)
cmake_minimum_required(VERSION 3.10)

aux_source_directory(src single_char_only_src)

add_library(rime-single_char_only-objs OBJECT ${single_char_only_src})
if(BUILD_SHARED_LIBS)
  set_target_properties(rime-single_char_only-objs
    PROPERTIES
    POSITION_INDEPENDENT_CODE ON)
endif()

set(single_char_only_library rime-single_char_only)
set(plugin_name ${single_char_only_library} PARENT_SCOPE)
set(plugin_objs $<TARGET_OBJECTS:rime-single_char_only-objs> PARENT_SCOPE)
set(single_char_only_deps
  ${rime_library}
  ${rime_gears_library}
)
set(plugin_deps ${single_char_only_deps} PARENT_SCOPE)
set(plugin_modules "single_char_only" PARENT_SCOPE)
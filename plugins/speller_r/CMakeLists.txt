project(rime-speller_r)
cmake_minimum_required(VERSION 3.10)

aux_source_directory(src speller_r_src)

add_library(rime-speller_r-objs OBJECT ${speller_r_src})
if(BUILD_SHARED_LIBS)
  set_target_properties(rime-speller_r-objs
    PROPERTIES
    POSITION_INDEPENDENT_CODE ON)
endif()

set(speller_r_library rime-speller_r)
set(plugin_name ${speller_r_library} PARENT_SCOPE)
set(plugin_objs $<TARGET_OBJECTS:rime-speller_r-objs> PARENT_SCOPE)
set(speller_r_deps
  ${rime_library}
  ${rime_gears_library}
)
set(plugin_deps ${speller_r_deps} PARENT_SCOPE)
set(plugin_modules "speller_r" PARENT_SCOPE)
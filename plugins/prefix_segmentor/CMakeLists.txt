project(rime-prefix_segmentor)
cmake_minimum_required(VERSION 3.10)

aux_source_directory(src prefix_segmentor_src)

add_library(rime-prefix_segmentor-objs OBJECT ${prefix_segmentor_src})
if(BUILD_SHARED_LIBS)
  set_target_properties(rime-prefix_segmentor-objs
    PROPERTIES
    POSITION_INDEPENDENT_CODE ON)
endif()

set(prefix_segmentor_library rime-prefix_segmentor)
set(plugin_name ${prefix_segmentor_library} PARENT_SCOPE)
set(plugin_objs $<TARGET_OBJECTS:rime-prefix_segmentor-objs> PARENT_SCOPE)
set(prefix_segmentor_deps
  ${rime_library}
  ${rime_gears_library}
)
set(plugin_deps ${prefix_segmentor_deps} PARENT_SCOPE)
set(plugin_modules "prefix_segmentor" PARENT_SCOPE)
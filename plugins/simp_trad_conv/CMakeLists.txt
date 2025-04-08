project(rime-simp_trad_conv)
cmake_minimum_required(VERSION 3.10)

if(NOT WIN32)
  find_package(Iconv)
  if (ICONV_FOUND)
    include_directories(${ICONV_INCLUDE_DIRS})
  endif()
endif()

aux_source_directory(src simp_trad_conv_src)

add_library(rime-simp_trad_conv-objs OBJECT ${simp_trad_conv_src})
if(BUILD_SHARED_LIBS)
  set_target_properties(rime-simp_trad_conv-objs
    PROPERTIES
    POSITION_INDEPENDENT_CODE ON)
endif()

set(simp_trad_conv_library rime-simp_trad_conv)
set(plugin_name ${simp_trad_conv_library} PARENT_SCOPE)
set(plugin_objs $<TARGET_OBJECTS:rime-simp_trad_conv-objs> PARENT_SCOPE)
set(simp_trad_conv_deps
  ${rime_library}
  ${rime_gears_library}
)
# 非Windows平台追加iconv库
if(NOT WIN32)
    list(APPEND simp_trad_conv_deps ${ICONV_LIBRARIES})
endif()
set(plugin_deps ${simp_trad_conv_deps} PARENT_SCOPE)
set(plugin_modules "simp_trad_conv" PARENT_SCOPE)
project(rime-often_charset_filter)
cmake_minimum_required(VERSION 3.10)

if(NOT WIN32)
  find_package(Iconv)
  if (ICONV_FOUND)
    include_directories(${ICONV_INCLUDE_DIRS})
  endif()
endif()

aux_source_directory(src often_charset_filter_src)

add_library(rime-often_charset_filter-objs OBJECT ${often_charset_filter_src})
if(BUILD_SHARED_LIBS)
  set_target_properties(rime-often_charset_filter-objs
    PROPERTIES
    POSITION_INDEPENDENT_CODE ON)
endif()

set(often_charset_filter_library rime-often_charset_filter)
set(plugin_name ${often_charset_filter_library} PARENT_SCOPE)
set(plugin_objs $<TARGET_OBJECTS:rime-often_charset_filter-objs> PARENT_SCOPE)
set(often_charset_filter_deps
  ${rime_library}
  ${rime_gears_library}
)
# 非Windows平台追加iconv库
if(NOT WIN32)
    list(APPEND often_charset_filter_deps ${ICONV_LIBRARIES})
endif()
set(plugin_deps ${often_charset_filter_deps} PARENT_SCOPE)
set(plugin_modules "often_charset_filter" PARENT_SCOPE)
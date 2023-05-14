project(rime-often_charset_filter)
cmake_minimum_required(VERSION 3.10)

option(BUILD_WITH_ICU "Build by explicitly linking to ICU libraries" OFF)

if (BUILD_WITH_ICU)
  find_package(ICU COMPONENTS uc)
endif()

find_package(Iconv)
if (ICONV_FOUND)
  include_directories(${ICONV_INCLUDE_DIRS})
endif()

find_package(Boost 1.46.0 REQUIRED COMPONENTS locale)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif()

aux_source_directory(src often_charset_filter_src)

add_library(rime-often_charset_filter-objs OBJECT ${often_charset_filter_src})
if(BUILD_SHARED_LIBS)
  set_target_properties(rime-often_charset_filter-objs
    PROPERTIES
    POSITION_INDEPENDENT_CODE ON)
endif()

# Boost::locale is not visible to parent. Create an interface library to work
# around. See https://stackoverflow.com/a/48741931
add_library(rime-often_charset_filter-deps INTERFACE)
target_link_libraries(rime-often_charset_filter-deps INTERFACE
  ${Boost_LOCALE_LIBRARIES}
  ${ICONV_LIBRARIES}
  ${ICU_LIBRARIES}   # 压根就没用上 icu
)

set(often_charset_filter_library rime-often_charset_filter)
set(plugin_name ${often_charset_filter_library} PARENT_SCOPE)
set(plugin_objs $<TARGET_OBJECTS:rime-often_charset_filter-objs> PARENT_SCOPE)
set(often_charset_filter_deps
  ${rime_library}
  ${rime_gears_library}
  rime-often_charset_filter-deps
)
set(plugin_deps ${often_charset_filter_deps} PARENT_SCOPE)
set(plugin_modules "often_charset_filter" PARENT_SCOPE)

#if(BUILD_TEST AND NOT BUILD_MERGED_PLUGINS)
#  add_subdirectory(test)
#endif()

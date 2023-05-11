project(rime-date_translator)
cmake_minimum_required(VERSION 3.10)

#option(BUILD_WITH_ICU "Build by explicitly linking to ICU libraries" ON)

#if (BUILD_WITH_ICU)
#  find_package(ICU COMPONENTS uc)
#endif()

find_package(Iconv)
if (ICONV_FOUND)
  include_directories(${ICONV_INCLUDE_DIRS})
endif()

find_package(Boost 1.46.0 REQUIRED COMPONENTS locale)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif()

aux_source_directory(src date_translator_src)

add_library(rime-date_translator-objs OBJECT ${date_translator_src})
if(BUILD_SHARED_LIBS)
  set_target_properties(rime-date_translator-objs
    PROPERTIES
    POSITION_INDEPENDENT_CODE ON)
endif()

# Boost::locale is not visible to parent. Create an interface library to work
# around. See https://stackoverflow.com/a/48741931
add_library(rime-date_translator-deps INTERFACE)
target_link_libraries(rime-date_translator-deps INTERFACE
  ${Boost_LOCALE_LIBRARIES}
  ${ICONV_LIBRARIES}
  #${ICU_LIBRARIES}
)

set(date_translator_library rime-date_translator)
set(plugin_name ${date_translator_library} PARENT_SCOPE)
set(plugin_objs $<TARGET_OBJECTS:rime-date_translator-objs> PARENT_SCOPE)
set(date_translator_deps
  ${rime_library}
  ${rime_gears_library}
  rime-date_translator-deps
)
set(plugin_deps ${date_translator_deps} PARENT_SCOPE)
set(plugin_modules "date_translator" PARENT_SCOPE)

#if(BUILD_TEST AND NOT BUILD_MERGED_PLUGINS)
#  add_subdirectory(test)
#endif()

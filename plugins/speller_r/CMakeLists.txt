project(rime-speller_r)
cmake_minimum_required(VERSION 3.10)

#option(BUILD_WITH_ICU "Build by explicitly linking to ICU libraries" OFF)

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

aux_source_directory(src speller_r_src)

add_library(rime-speller_r-objs OBJECT ${speller_r_src})
if(BUILD_SHARED_LIBS)
  set_target_properties(rime-speller_r-objs
    PROPERTIES
    POSITION_INDEPENDENT_CODE ON)
endif()

# Boost::locale is not visible to parent. Create an interface library to work
# around. See https://stackoverflow.com/a/48741931
add_library(rime-speller_r-deps INTERFACE)
target_link_libraries(rime-speller_r-deps INTERFACE
  ${Boost_LOCALE_LIBRARIES}
  ${ICONV_LIBRARIES}
  #${ICU_LIBRARIES}
)

set(speller_r_library rime-speller_r)
set(plugin_name ${speller_r_library} PARENT_SCOPE)
set(plugin_objs $<TARGET_OBJECTS:rime-speller_r-objs> PARENT_SCOPE)
set(speller_r_deps
  ${rime_library}
  ${rime_gears_library}
  rime-speller_r-deps
)
set(plugin_deps ${speller_r_deps} PARENT_SCOPE)
set(plugin_modules "speller_r" PARENT_SCOPE)

#if(BUILD_TEST AND NOT BUILD_MERGED_PLUGINS)
#  add_subdirectory(test)
#endif()

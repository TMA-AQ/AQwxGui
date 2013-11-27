cmake_minimum_required(VERSION 2.8)

project(AQTools)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

find_package(Boost 1.48.0 COMPONENTS program_options thread filesystem system timer regex REQUIRED)
find_package(wxWidgets REQUIRED core base)

include(${wxWidgets_USE_FILE})

include_directories(
  ${PROJECT_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${wxWidgets_INCLUDE_DIRS}
  $ENV{AQUTIL_SOURCE_DIR}
  $ENV{AQQUERY_RESOLVER_SOURCE_DIR}
)

link_directories(
  ${LIBRARY_OUTPUT_PATH}
  ${Boost_LIBRARY_DIRS}
  ${wxWidgets_LIBRARY_DIRS}
  $ENV{AQUTIL_LIBRARY_DIR}
  $ENV{AQQUERY_RESOLVER_LIBRARY_DIR}
)

set(CMAKE_CXX_FLAGS "-O0 -ggdb -std=c++11 ${wxWidgets_CXXFLAGS}")

file(GLOB_RECURSE source_files src/*.cpp)

set(MACOSX_BUNDLE_NAME aq-gui)
add_executable(aq-gui ${source_files})
set_target_properties(aq-gui PROPERTIES MACOSX_BUNDLE TRUE)

target_link_libraries(
  aq-gui
  aq_query_resolver
  aq_util
  ${Boost_LIBRARIES}
  ${wxWidgets_LIBRARIES}
)

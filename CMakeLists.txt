cmake_minimum_required(VERSION 2.8)

project(AQwxGui)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.48.0 COMPONENTS program_options thread filesystem system timer regex REQUIRED)
find_package(wxWidgets REQUIRED core base)

include(${wxWidgets_USE_FILE})

include_directories(
  ${PROJECT_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${wxWidgets_INCLUDE_DIRS}
  ${AQUTIL_INCLUDE_DIRS}
  ${AQQUERY_RESOLVER_INCLUDE_DIRS}
  ${AQTESTS_INCLUDE_DIRS}
)

link_directories(
  ${LIBRARY_OUTPUT_PATH}
  ${Boost_LIBRARY_DIRS}
  ${wxWidgets_LIBRARY_DIRS}
  ${AQUTIL_LIBRARY_DIRS}
  ${AQQUERY_RESOLVER_LIBRARY_DIRS}
  ${AQTESTS_LIBRARY_DIRS}
)

set(CMAKE_CXX_FLAGS "-O0 -ggdb -std=c++11 -stdlib=libc++ ${wxWidgets_CXXFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-lc++")

file(GLOB_RECURSE source_files src/*.cpp)

set(MACOSX_BUNDLE_NAME aq-gui)
add_executable(aq-gui ${source_files})
set_target_properties(aq-gui PROPERTIES MACOSX_BUNDLE TRUE)

target_link_libraries(
  aq-gui
  aq-tests
  aq-query-resolver
  aq-engine
  aq-util
  ${Boost_LIBRARIES}
  ${wxWidgets_LIBRARIES}
  mysqlcppconn-static.a
  /usr/local/mysql/lib/libmysqlclient.a
)

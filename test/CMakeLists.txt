cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(UnknownTwitchGameTests LANGUAGES CXX)

# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage("gh:TheLartians/Format.cmake@1.7.0")

if(TEST_INSTALLED_VERSION)
  find_package(UnknownTwitchGame REQUIRED)
else()
  CPMAddPackage(NAME UnknownTwitchGame SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

# ---- Create binary ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
# add_executable(UnknownTwitchGameTests ${sources})
# target_link_libraries(UnknownTwitchGameTests UnknownTwitchGame::UnknownTwitchGame)
# set_target_properties(UnknownTwitchGameTests PROPERTIES CXX_STANDARD 17)

# enable compiler warnings
if(NOT TEST_INSTALLED_VERSION)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(UnknownTwitchGame PUBLIC -Wall -Wpedantic -Wextra -Werror)
  elseif(MSVC)
    target_compile_options(UnknownTwitchGame PUBLIC /W4 /WX)
  endif()
endif()

# ---- Add UnknownTwitchGameTests ----

enable_testing()

# Note: doctest and similar testing frameworks can automatically configure CMake tests. For other
# testing frameworks add the tests target instead: add_test(NAME UnknownTwitchGameTests COMMAND UnknownTwitchGameTests)

# ---- code coverage ----

if(ENABLE_TEST_COVERAGE)
  target_compile_options(UnknownTwitchGame PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(UnknownTwitchGame PUBLIC -fprofile-arcs -ftest-coverage)
endif()

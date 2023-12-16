# Example usage:
#  mkdir build
#  cd build
#  cmake -g "Visual Studio 14 2015" ..

cmake_minimum_required(VERSION 3.4)
project(AdventOfCode2023 C CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")

if ( CMAKE_COMPILER_IS_GNUCC )
	# TODO: add -Werror and fix all warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Wuninitialized -Wpedantic -Wno-unknown-pragmas -Wunused-parameter -fPIC -Werror")
  if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-variable")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
  endif()
endif ( CMAKE_COMPILER_IS_GNUCC )
if ( MSVC )
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /MP")
endif ( MSVC )

set(SOURCE_DIR src)
set(DATA_DIR data)

file(GLOB SRC_DATA "${DATA_DIR}/*")

file(GLOB SRC_DAY1 "${SOURCE_DIR}/day1.cpp" "${DATA_DIR}/day1_*")
file(GLOB SRC_DAY2 "${SOURCE_DIR}/day2.cpp" "${DATA_DIR}/day2_*")
file(GLOB SRC_DAY3 "${SOURCE_DIR}/day3.cpp" "${DATA_DIR}/day3_*")
file(GLOB SRC_DAY4 "${SOURCE_DIR}/day4.cpp" "${DATA_DIR}/day4_*")
file(GLOB SRC_DAY5 "${SOURCE_DIR}/day5.cpp" "${DATA_DIR}/day5_*")
file(GLOB SRC_DAY6 "${SOURCE_DIR}/day6.cpp" "${DATA_DIR}/day6_*")
file(GLOB SRC_DAY7 "${SOURCE_DIR}/day7.cpp" "${DATA_DIR}/day7_*")
file(GLOB SRC_DAY8 "${SOURCE_DIR}/day8.cpp" "${DATA_DIR}/day8_*")
file(GLOB SRC_DAY9 "${SOURCE_DIR}/day9.cpp" "${DATA_DIR}/day9_*")
file(GLOB SRC_DAY10 "${SOURCE_DIR}/day10.cpp" "${DATA_DIR}/day10_*")
file(GLOB SRC_DAY11 "${SOURCE_DIR}/day11.cpp" "${DATA_DIR}/day11_*")
file(GLOB SRC_DAY12 "${SOURCE_DIR}/day12.cpp" "${DATA_DIR}/day12_*")
file(GLOB SRC_DAY13 "${SOURCE_DIR}/day13.cpp" "${DATA_DIR}/day13_*")
file(GLOB SRC_DAY14 "${SOURCE_DIR}/day14.cpp" "${DATA_DIR}/day14_*")

source_group("data"  FILES ${SRC_DATA})

add_executable(day01 ${SRC_DAY1})
add_executable(day02 ${SRC_DAY2})
add_executable(day03 ${SRC_DAY3})
add_executable(day04 ${SRC_DAY4})
add_executable(day05 ${SRC_DAY5})
add_executable(day06 ${SRC_DAY6})
add_executable(day07 ${SRC_DAY7})
add_executable(day08 ${SRC_DAY8})
add_executable(day09 ${SRC_DAY9})
add_executable(day10 ${SRC_DAY10})
add_executable(day11 ${SRC_DAY11})
add_executable(day12 ${SRC_DAY12})
add_executable(day13 ${SRC_DAY13})
add_executable(day14 ${SRC_DAY14})
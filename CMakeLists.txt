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

file(GLOB SRC_DAY1 "${SOURCE_DIR}/day1.cpp" "${DATA_DIR}/day1*")
file(GLOB SRC_DAY2 "${SOURCE_DIR}/day2.cpp" "${DATA_DIR}/day2*")
file(GLOB SRC_DAY3 "${SOURCE_DIR}/day3.cpp" "${DATA_DIR}/day3*")
file(GLOB SRC_DAY4 "${SOURCE_DIR}/day4.cpp" "${DATA_DIR}/day4*")
file(GLOB SRC_DAY5 "${SOURCE_DIR}/day5.cpp" "${DATA_DIR}/day5*")

#set(SRC_DAY3 "${SOURCE_DIR}/day3.cpp" "${DATA_DIR}/day3*" )

source_group("data"  FILES ${SRC_DATA})

add_executable(day1 ${SRC_DAY1})
add_executable(day2 ${SRC_DAY2})
add_executable(day3 ${SRC_DAY3})
add_executable(day4 ${SRC_DAY4})
add_executable(day5 ${SRC_DAY5})
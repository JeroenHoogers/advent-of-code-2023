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
source_group("data"  FILES ${SRC_DATA})

# Increase this number every day
set(DAYS 17)

foreach(DAY RANGE 1 ${DAYS})
  # Correct ordering with leading zero
  set(EXE_NAME ${DAY})
  if(${EXE_NAME} LESS "10")
    string(PREPEND EXE_NAME "0")
  endif()

  file(GLOB SRC_DAY${DAY} "${SOURCE_DIR}/day${DAY}.cpp" "${DATA_DIR}/day${DAY}_*")
  add_executable(day${EXE_NAME} ${SRC_DAY${DAY}})
endforeach(DAY)

cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-lboost_system -lpthread")

find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
find_package( Threads )

add_executable(starter "starter.cpp")
target_link_libraries(starter ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})


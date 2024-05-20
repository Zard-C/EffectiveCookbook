find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(widget main.cpp widget.cpp)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(use_make_shared use_make_shared.cpp)
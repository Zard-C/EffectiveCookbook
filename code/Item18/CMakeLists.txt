find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(use_unique_ptr use_unique_ptr.cpp)

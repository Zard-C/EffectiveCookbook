find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(create_object create_object.cpp)
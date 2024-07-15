find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(forward_failed forward_failed.cpp)
target_link_libraries(forward_failed ${Boost_LIBRARIES})
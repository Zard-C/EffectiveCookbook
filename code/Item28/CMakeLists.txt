find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(reference_collapsing reference_collapsing.cpp)
target_link_libraries(reference_collapsing ${Boost_LIBRARIES})
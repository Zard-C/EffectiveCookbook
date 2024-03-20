find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(use_alias use_alias.cpp)
target_link_libraries(use_alias ${Boost_LIBRARIES})

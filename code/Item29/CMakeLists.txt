find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(move_drawbacks move_drawbacks.cpp)
target_link_libraries(move_drawbacks ${Boost_LIBRARIES})
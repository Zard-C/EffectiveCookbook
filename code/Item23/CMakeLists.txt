find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(move_and_forward move_and_forward.cpp)
target_link_libraries(move_and_forward ${Boost_LIBRARIES})
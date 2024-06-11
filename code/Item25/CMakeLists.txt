find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(use_move_and_forward use_move_and_forward.cpp)
target_link_libraries(use_move_and_forward ${Boost_LIBRARIES})
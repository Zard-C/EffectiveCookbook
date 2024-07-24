find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set (CMAKE_CXX_STANDARD 14)

add_executable(init_capture init_capture.cpp)
target_link_libraries(init_capture ${Boost_LIBRARIES})
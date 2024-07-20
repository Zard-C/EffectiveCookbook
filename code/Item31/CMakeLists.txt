find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(avoid_default_capture avoid_default_capture.cpp)
target_link_libraries(avoid_default_capture ${Boost_LIBRARIES})
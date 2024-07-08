find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(decltype decltype.cpp)
target_link_libraries(decltype ${Boost_LIBRARIES})
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(use_auto_and_decltype use_auto_and_decltype.cpp)
target_link_libraries(use_auto_and_decltype ${Boost_LIBRARIES})
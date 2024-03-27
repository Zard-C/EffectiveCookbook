find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(use_const_iterator use_const_iterator.cpp)
target_link_libraries(use_const_iterator ${Boost_LIBRARIES})
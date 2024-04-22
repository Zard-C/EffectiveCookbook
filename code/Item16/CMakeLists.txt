find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(const_member const_member.cpp)
target_link_libraries(const_member ${Boost_LIBRARIES} pthread)
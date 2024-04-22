find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(special_member_functions special_member_functions.cpp)
target_link_libraries(special_member_functions ${Boost_LIBRARIES})
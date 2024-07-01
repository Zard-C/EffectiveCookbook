find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})


add_executable(tag_dispatch tag_dispatch.cpp)
target_link_libraries(tag_dispatch ${Boost_LIBRARIES})
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(enum enum.cpp)
target_link_libraries(enum ${Boost_LIBRARIES})
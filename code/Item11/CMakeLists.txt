find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(use_delete_functions use_delete_functions.cpp)
target_link_libraries(use_delete_functions ${Boost_LIBRARIES})
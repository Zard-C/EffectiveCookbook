find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(auto auto.cpp)
target_link_libraries(auto ${Boost_LIBRARIES})

add_executable(pointer_or_reference condition1.cpp)
target_link_libraries(pointer_or_reference ${Boost_LIBRARIES})

add_executable(universal condition2.cpp)
target_link_libraries(universal ${Boost_LIBRARIES})

add_executable(neither condition3.cpp)
target_link_libraries(neither ${Boost_LIBRARIES})

add_executable(initializer_list initializer_list.cpp)
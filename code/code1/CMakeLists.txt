find_package(Boost REQUIRED)
add_executable(condition1 src/condition1.cpp)
target_link_libraries(condition1 ${Boost_LIBRARIES})

add_executable(condition2 src/condition2.cpp)
target_link_libraries(condition2 ${Boost_LIBRARIES})

add_executable(condition3 src/condition3.cpp)
target_link_libraries(condition3 ${Boost_LIBRARIES})

add_executable(array_params src/array_params.cpp)
target_link_libraries(array_params ${Boost_LIBRARIES})

add_executable(func_params src/func_params.cpp)
target_link_libraries(func_params ${Boost_LIBRARIES})
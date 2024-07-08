find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(type_deduct type_deduction.cpp)
target_link_libraries(type_deduct ${Boost_LIBRARIES})
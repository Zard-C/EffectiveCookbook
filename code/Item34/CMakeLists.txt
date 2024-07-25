find_package(Boost  REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(prefer_lambda prefer_lambda.cpp)
target_link_libraries(prefer_lambda ${Boost_LIBRARIES})
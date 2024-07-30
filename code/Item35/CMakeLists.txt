find_package(Boost  REQUIRED)
find_package(Threads REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(prefer_task_based_programming prefer_task_based_programming.cpp)
target_link_libraries(prefer_task_based_programming ${Boost_LIBRARIES}  Threads::Threads)
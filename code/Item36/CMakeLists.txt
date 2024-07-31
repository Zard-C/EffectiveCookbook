find_package(Boost REQUIRED)
find_package(Threads REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})


add_executable(specify_std_launch_async specify_std_launch_async.cpp)
target_link_libraries(specify_std_launch_async ${Boost_LIBRARIES} Threads::Threads)
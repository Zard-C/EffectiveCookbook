find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(rref_and_universal_ref rref_and_universal_ref.cpp)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(use_constexpr use_constexpr.cpp)
target_link_libraries(use_constexpr  )
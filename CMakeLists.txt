cmake_minimum_required(VERSION 3.22.1)
project(cCalcApp LANGUAGES C VERSION 1.0)

# Pick C17 and good warnings everywhere
set(CMAKE_C_STANDARD            17)
set(CMAKE_C_STANDARD_REQUIRED   ON)
add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(main main.c)

# libm is needed only on POSIX
if(NOT WIN32)
    target_link_libraries(main m)
endif()

# Optional: install target (cmake --install <build>)
install(TARGETS main RUNTIME DESTINATION bin)

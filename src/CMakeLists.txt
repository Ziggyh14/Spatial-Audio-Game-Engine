cmake_minimum_required(VERSION 3.15)
project(hush VERSION 0.2)

set(C_STANDARD 11)
set(C_STANDARD_REQUIRED True)

add_subdirectory(lib/SDL2 EXCLUDE_FROM_ALL)
add_subdirectory(lib/openal-soft EXCLUDE_FROM_ALL)

add_executable(out main.c
        sdl_func.c
        sample_func.c
        sample_hash.c
        state_manager.c
        event_handler.c
        collision_util.c)

target_include_directories(out PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(out PUBLIC SDL2::SDL2 SDL2::SDL2main OpenAL::OpenAL m)

cmake_minimum_required(VERSION 3.9)

project(hush VERSION 0.8)


add_library(hush SHARED
        src/sdl_func.c
        src/state_manager.c
        src/event_handler.c
        src/collision_util.c
        src/sample_hash.c
        src/sample_func.c
        src/sample_queue.c

        src/sdl_func.h
        src/state_manager.h
        src/collision_util.h
        src/sample_func.h

)

set_target_properties(hush PROPERTIES PUBLIC_HEADER src/hush.h)

#add_subdirectory(lib/SDL2 EXCLUDE_FROM_ALL)
add_subdirectory(lib/openal-soft EXCLUDE_FROM_ALL)
add_subdirectory(lib/SDL_sound EXCLUDE_FROM_ALL)
find_package(SDL2 REQUIRED)

include(GNUInstallDirs)

target_link_libraries(hush PUBLIC SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(hush PUBLIC SDL2_sound::SDL2_sound OpenAL::OpenAL m)

install(TARGETS hush
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})



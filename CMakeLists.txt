cmake_minimum_required(VERSION 3.9)

project(hush VERSION 0.8)

include(GNUInstallDirs)

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
        src/hush.h
)
add_library(hush::hush ALIAS hush)

#set_target_properties(hush PROPERTIES PUBLIC_HEADER src/hush.h)
#target_include_directories(hush PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)


#add_subdirectory(lib/SDL2 EXCLUDE_FROM_ALL)
add_subdirectory(lib/openal-soft )
add_subdirectory(lib/SDL_sound)
find_package(SDL2 REQUIRED)

target_include_directories(hush PUBLIC 
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_link_libraries(hush PUBLIC SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(hush PUBLIC SDL2_sound::SDL2_sound OpenAL::OpenAL m)

configure_file(hush.pc.in hush.pc @ONLY)

install(TARGETS hush
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES src/hush.h  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${CMAKE_BINARY_DIR}/hush.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)



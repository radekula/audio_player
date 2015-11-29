cmake_minimum_required (VERSION 3.0)
project (audio_player)

set (audio_player_VERSION_MAJOR 0)
set (audio_player_VERSION_MINOR 1)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 3.0)
    if (NOT DEFINED CMAKE_CXX_STANDARD)
        set(CMAKE_CXX_STANDARD 14)
    endif ()
else ()
    set (CMAKE_CXX_FLAGS "-std=c++14")
endif ()


find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-0.10)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

include_directories(${GSTREAMER_INCLUDE_DIRS})
link_directories(${GSTREAMER_LIBRARY_DIRS})

include_directories (${CMAKE_CURRENT_LIST_DIR}/src/audio_player/)


file (GLOB AUDIO_PLAYER_SRC "src/audio_player/*.cpp")
file (GLOB GUI_GTK_SRC "src/gui_libs/gui_gtk.cpp")
file (GLOB GUI_TERMINAL_SRC "src/gui_libs/gui_terminal.cpp")

add_library (gui_gtk SHARED ${GUI_GTK_SRC})
add_library (gui_terminal SHARED ${GUI_TERMINAL_SRC})

add_executable (audio_player ${AUDIO_PLAYER_SRC})
target_link_libraries(audio_player ${GTK3_LIBRARIES} dl pthread)
target_link_libraries(audio_player ${GSTREAMER_LIBRARIES} dl pthread)

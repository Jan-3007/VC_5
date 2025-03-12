

include_guard(GLOBAL)

add_library(paint_lib INTERFACE)

target_include_directories(
    paint_lib INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/Config
    ${CMAKE_CURRENT_LIST_DIR}/Fonts
    ${CMAKE_CURRENT_LIST_DIR}/GUI
    )

target_sources(
    paint_lib INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/Fonts/font8.c
    ${CMAKE_CURRENT_LIST_DIR}/Fonts/font12.c
    ${CMAKE_CURRENT_LIST_DIR}/Fonts/font12CN.c
    ${CMAKE_CURRENT_LIST_DIR}/Fonts/font16.c
    ${CMAKE_CURRENT_LIST_DIR}/Fonts/font20.c
    ${CMAKE_CURRENT_LIST_DIR}/Fonts/font24.c
    ${CMAKE_CURRENT_LIST_DIR}/Fonts/font24CN.c
    ${CMAKE_CURRENT_LIST_DIR}/GUI/GUI_Paint.c
)


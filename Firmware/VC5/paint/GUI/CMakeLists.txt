add_executable(GUI
    GUI_Paint.c
    )

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(GUI)




#pico_add_library(GUI_Paint)

#target_sources(GUI_Paint INTERFACE
#        GUI_Paint.c
#        )

#target_include_directories(GUI_Paint_headers INTERFACE ${CMAKE_CURRENT_LIST_DIR})

#pico_mirrored_target_link_libraries(pico_unique_id INTERFACE hardware_flash)
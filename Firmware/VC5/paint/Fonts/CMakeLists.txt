add_executable(Fonts
    font8.c
    font12.c
    font12CN.c
    font16.c
    font20.c
    font24.c
    font24CN.c
    )

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(Fonts)


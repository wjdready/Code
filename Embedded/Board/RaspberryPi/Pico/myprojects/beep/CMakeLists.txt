add_executable(beep
        main.c
        test_command.c
        test.c
        )

target_link_libraries(beep
        pico_stdlib
        hardware_flash
        hardware_pwm
        )

# create map/bin/hex file etc.
pico_add_extra_outputs(beep)

# add url via pico_set_program_url
# example_auto_set_url(beep)

add_executable(flash_program
        flash_program.c
        test_command.c
        test.c
        )

target_link_libraries(flash_program
        pico_stdlib
        hardware_flash
        hardware_pwm
        )

# create map/bin/hex file etc.
pico_add_extra_outputs(flash_program)

# add url via pico_set_program_url
# example_auto_set_url(flash_program)

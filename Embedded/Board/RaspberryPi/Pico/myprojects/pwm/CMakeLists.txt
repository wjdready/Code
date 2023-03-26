add_executable(pwm
    main.c
    test_command.c
)

target_link_libraries(pwm
    pico_stdlib
    hardware_pwm
)

# create map/bin/hex file etc.
pico_add_extra_outputs(pwm)

# example_auto_set_url(pwm)

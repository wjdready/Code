add_executable(demo
    main.c
    test_command.c
)

target_link_libraries(demo
    pico_stdlib
)

# create map/bin/hex file etc.
pico_add_extra_outputs(demo)

# example_auto_set_url(demo)

get_filename_component(CURRENT_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH) 

find_path(AWALib_INCLUDE_DIR 
    NAMES awalib.h
    HINTS ${CURRENT_CMAKE_DIR}
)

if(WIN32)
    set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .dll)
else()
   set(CMAKE_FIND_LIBRARY_SUFFIXES .a .so)
endif()

find_library(AWALib_LIBRARIES
    NAMES awalib  
    PATHS ${CURRENT_CMAKE_DIR}
)

if (AWALib_INCLUDE_DIR AND AWALib_LIBRARIES)
    set(AWALib_FOUND TRUE)

    if(NOT TARGET AWALib::AWALib)
        add_library(AWALib::AWALib INTERFACE IMPORTED)
        set_target_properties(AWALib::AWALib PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${AWALib_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${AWALib_LIBRARIES}")
    endif()
endif ()

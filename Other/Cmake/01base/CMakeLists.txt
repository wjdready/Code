cmake_minimum_required(VERSION 3.20)

project(myapp VERSION 1.0)

# add_subdirectory 的同时也编译并获取了子目录下的 target
add_subdirectory(add)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_BUILD_TYPE "Release")

include_directories(add/include)
add_executable(${CMAKE_PROJECT_NAME} main.c)
# target_link_libraries(${CMAKE_PROJECT_NAME} add)
target_link_libraries(${CMAKE_PROJECT_NAME} add_static)

install(TARGETS add DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

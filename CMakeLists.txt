cmake_minimum_required(VERSION 3.10)
project(IndianSpaceProgram)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:/Users/yakun/OneDrive/SFML/SFML++/SFML2.0/SFML-2.6.1-windows-vc17-64-bit/SFML-2.6.1/lib/cmake/SFML")
set(SFML_INCLUDE_DIR "C:/Users/yakun/OneDrive/SFML/SFML++/SFML2.0/SFML-2.6.1-windows-vc17-64-bit/SFML-2.6.1/include")
set(SFML_LIB_DIR "C:/Users/yakun/OneDrive/SFML/SFML++/SFML2.0/SFML-2.6.1-windows-vc17-64-bit/SFML-2.6.1/lib")

# Убрали main отсюда
find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)

add_executable(Game src/main.cpp src/Game.cpp)

target_include_directories(Game PRIVATE include "${SFML_INCLUDE_DIR}")
target_link_directories(Game PRIVATE "${SFML_LIB_DIR}")

# Убрали sfml-main и sfml-main-d отсюда
target_link_libraries(Game PRIVATE 
    debug sfml-graphics-d optimized sfml-graphics
    debug sfml-window-d optimized sfml-window
    debug sfml-system-d optimized sfml-system
    debug sfml-audio-d optimized sfml-audio
) 

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()

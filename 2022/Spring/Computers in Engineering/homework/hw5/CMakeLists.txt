cmake_minimum_required(VERSION 3.10)
project(DoodleJump)

set(CMAKE_CXX_STANDARD 11)

configure_file( images/background.png images/background.png COPYONLY)
configure_file( images/doodle.png images/doodle.png COPYONLY)
configure_file( images/platform.png images/platform.png COPYONLY)
configure_file( images/platform_bad.png images/platform_bad.png COPYONLY)
configure_file( sounds/jump.ogg sounds/jump.ogg COPYONLY)
configure_file( sounds/fall.ogg sounds/fall.ogg COPYONLY)
configure_file( sounds/background.ogg sounds/background.ogg COPYONLY)
configure_file( fonts/ConcertOne-Regular.ttf fonts/ConcertOne-Regular.ttf COPYONLY)
configure_file( fonts/RobotoMono-Bold.ttf fonts/RobotoMono-Bold.ttf COPYONLY)
configure_file( ath_score.txt ath_score.txt COPYONLY)

# Define sources and executable
set(EXECUTABLE_NAME "DoodleJump")

add_executable(${EXECUTABLE_NAME} main.cpp DoodleFunctions.h DoodleFunctions.cpp)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()
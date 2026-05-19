cmake_minimum_required(VERSION 3.16)
project(project-1 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Create a symlink at the end of the configuration step
add_custom_target(copy_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/compile_commands.json"
        "${CMAKE_SOURCE_DIR}/compile_commands.json"
)

find_package(OpenCV REQUIRED)

add_executable(vidDisplay
    ./src/vidDisplay.cpp
    ./src/filter.cpp
    ./include/faceDetect.cpp
)
target_link_libraries(vidDisplay PRIVATE ${OpenCV_LIBS})
target_include_directories(vidDisplay PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ./include
)

add_executable(imgDisplay ./src/imgDisplay.cpp)
target_link_libraries(imgDisplay PRIVATE ${OpenCV_LIBS})
target_include_directories(imgDisplay PRIVATE ${OpenCV_INCLUDE_DIRS})

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

find_path(ONNXRUNTIME_INCLUDE_DIR
    NAMES onnxruntime_cxx_api.h
    HINTS $ENV{ONNXRUNTIME_ROOT}/include
    REQUIRED
)

find_library(ONNXRUNTIME_LIB
    NAMES onnxruntime
    HINTS $ENV{ONNXRUNTIME_ROOT}/lib
    REQUIRED
)

message(STATUS "onnxruntime include: ${ONNXRUNTIME_INCLUDE_DIR}")
message(STATUS "onnxruntime lib:     ${ONNXRUNTIME_LIB}")

# vidDisplay — opencv only
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

# da2-video — opencv + onnxruntime (manual)
add_executable(da2-video ./include/da2-video.cpp)
target_link_libraries(da2-video PRIVATE
    ${OpenCV_LIBS}
    ${ONNXRUNTIME_LIB}
)
target_include_directories(da2-video PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${ONNXRUNTIME_INCLUDE_DIR}
    ./include
)

# Copy onnxruntime.dll next to da2-video.exe
add_custom_command(TARGET da2-video POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$ENV{ONNXRUNTIME_ROOT}/lib/onnxruntime.dll"
        "$<TARGET_FILE_DIR:da2-video>"
)

add_executable(imgDisplay ./src/imgDisplay.cpp)
target_link_libraries(imgDisplay PRIVATE ${OpenCV_LIBS})
target_include_directories(imgDisplay PRIVATE ${OpenCV_INCLUDE_DIRS})

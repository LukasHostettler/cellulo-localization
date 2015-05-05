find_package(OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})
message(STATUS "OpenCV version: ${OpenCV_VERSION}")

include_directories(../include)
include_directories(../test)

file(GLOB mouse_source_files *.cpp)
add_executable(mouse_driver ${mouse_source_files})

target_link_libraries(mouse_driver cellulo_localization ${OpenCV_LIBS} m)

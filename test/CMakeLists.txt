
find_package(OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})
message(STATUS "OpenCV version: ${OpenCV_VERSION}")

include_directories(../include)
include_directories(../src)

file(GLOB cellulo_localization_demo_source_files *.cpp)
add_executable(localization_test ${cellulo_localization_demo_source_files})

target_link_libraries(localization_test cellulo_localization ${OpenCV_LIBS})



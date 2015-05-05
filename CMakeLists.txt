cmake_minimum_required(VERSION 2.8)

project(cellulo)

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")


file(GLOB cellulo_localization_source_files ${CMAKE_SOURCE_DIR}/src/*.c)
add_library(cellulo_localization SHARED ${cellulo_localization_source_files})

install (TARGETS cellulo_localization
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)


option(WITH_TESTS                     "Build testing apps" OFF)
option(WITH_DEMOS                     "Build demo apps" OFF)
option(WITH_MOUSE		      "Build mouse driver" OFF)

if (WITH_TESTS)
    add_subdirectory(test)
endif()

if(WITH_DEMOS)
    add_subdirectory(demo)
endif()

if(WITH_MOUSE)
	add_subdirectory(mouse)
endif()




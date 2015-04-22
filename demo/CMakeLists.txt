
include_directories(../include)

file(GLOB demo_source_files *.c)
add_executable(localization_demo ${demo_source_files})

target_link_libraries(localization_demo cellulo_localization)



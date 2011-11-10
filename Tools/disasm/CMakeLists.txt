message("Build directory: ${CMAKE_BINARY_DIR}")
add_executable(disasm disasm.cpp)
target_link_libraries(disasm dviview)

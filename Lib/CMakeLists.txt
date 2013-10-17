set(DVIVIEW_BUILD_DIR "${DVIVIEW_BUILD_DIR}Lib/")

set(DVIVIEW_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/DVIFile.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/PKFile.cpp")

add_subdirectory(Ancillary "${DVIVIEW_BUILD_DIR}Ancillary")

add_library(dviview ${DVIVIEW_LIB_SOURCES})

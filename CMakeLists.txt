cmake_minimum_required (VERSION 2.6)
project (DVIVIEW)

# c config
set(OPT "-g -O0")
add_definitions(${OPT} -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function)
include_directories(Include)

# set up miscellaneous variables
set(DVIVIEW_BUILD_DIR build/)


# add subdirectory

add_subdirectory(Lib Build/Lib)
add_subdirectory(Tools Build/Tools) 


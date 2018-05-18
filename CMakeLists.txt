cmake_minimum_required(VERSION 2.8)

# Name of the project (will be the name of the plugin)
project(EigenJs)


# Build a shared library named after the project from the files in `src/`
file(GLOB SOURCE_FILES "src/*.cpp" "src/*.hpp")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")


# Essential include files to build a node addon,
# You should add this line in every CMake.js based project
target_include_directories(
	${PROJECT_NAME}
PRIVATE
	${CMAKE_JS_INC}
	src/Eigen
)

# Essential library files to link to a node addon
# You should add this line in every CMake.js based project
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

#target_link_libraries(${PROJECT_NAME} ${OpenBLAS_LIBNAME})

#add_custom_command(TARGET ${PROJECT_NAME}
#	POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy $<$<TARGET_FILE_NAME:${OpenBLAS_LIBNAME}> ${CMAKE_CURRENT_BINARY_DIR}/.
#)
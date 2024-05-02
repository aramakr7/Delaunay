cmake_minimum_required(VERSION 3.14)
project(TriangulateInsertion)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE on)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall")

# Set the source directory and the build output directory
set(SRCDIR ${CMAKE_SOURCE_DIR})
set(BUILDDIR ${CMAKE_BINARY_DIR})

# Specify the executable target and source files
file(GLOB SOURCES "${SRCDIR}/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# Find Python and NumPy
find_package(Python3 COMPONENTS Interpreter Development)
find_package(Python3 COMPONENTS NumPy)
if(Python3_FOUND AND Python3_NumPy_FOUND)
  target_include_directories(${PROJECT_NAME} PRIVATE ${Python3_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PRIVATE Python3::Python)
  target_include_directories(${PROJECT_NAME} PRIVATE ${Python3_NumPy_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PRIVATE Python3::NumPy)
  target_compile_definitions(${PROJECT_NAME} PRIVATE WITH_MATPLOTLIB)
endif()

# Set the output directory for executable files
set_target_properties(${PROJECT_NAME} PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${BUILDDIR})

# Custom target for cleaning additional files
add_custom_target(clean-extra
                  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/clean_all.cmake)

# Create a CMake script to remove build artifacts
file(WRITE ${CMAKE_BINARY_DIR}/clean_all.cmake
     "file(REMOVE_RECURSE ${BUILDDIR} ${CMAKE_BINARY_DIR}/${PROJECT_NAME})")

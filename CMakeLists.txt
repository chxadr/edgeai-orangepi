cmake_minimum_required(VERSION 3.10.0)
project(OrangePiVision VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

message(STATUS "Project source dir: ${PROJECT_SOURCE_DIR}")

message(STATUS "Looking for packages...")
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED libgpiod)
message(STATUS "Found all packages")

# Collect sources
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.c")
message(STATUS "Project source files: ${SOURCE_FILES}")

# Create shared library
add_library(c_interface SHARED ${SOURCE_FILES})

# Includes
target_include_directories(c_interface PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
    ${GPIOD_INCLUDE_DIRS}
)

# Compile flags
target_compile_options(c_interface PRIVATE ${GPIOD_CFLAGS_OTHER})

# Link libraries
target_link_libraries(c_interface PRIVATE
    ${OpenCV_LIBS}
    ${GPIOD_LIBRARIES}
    pthread
    dl
    m
)

# Optionally install
# install(TARGETS c_interface LIBRARY DESTINATION lib)

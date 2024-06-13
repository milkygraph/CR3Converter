cmake_minimum_required(VERSION 3.27)
project("CR3Converter")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find and include GLFW
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})

# Find and include GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})

# Find and include OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# Find and include LibRaw
pkg_check_modules(LIBRAW REQUIRED libraw)
include_directories(${LIBRAW_INCLUDE_DIRS})
link_directories(${LIBRAW_LIBRARY_DIRS})

# Include glm
include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

include_directories(include)
include_directories(src/imgui)
include_directories(src/imgui/backends)

FILE(GLOB_RECURSE SRC_FILES src/*.cpp)
FILE(GLOB IMGUI_FILES src/imgui/*.cpp src/imgui/backends/imgui_impl_glfw.cpp src/imgui/backends/imgui_impl_opengl3.cpp)

add_executable(CR3Converter ${SRC_FILES} ${IMGUI_FILES})

target_link_libraries(CR3Converter ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${LIBRAW_LIBRARIES} glm::glm)

cmake_minimum_required(VERSION 3.16)
project(ThePlazza CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(plazzaInclude INTERFACE)

target_include_directories(plazzaInclude INTERFACE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/encapsulation
    ${PROJECT_SOURCE_DIR}/include/kitchen
    ${PROJECT_SOURCE_DIR}/include/pizza
    ${PROJECT_SOURCE_DIR}/include/reception
)

file(GLOB_RECURSE CORE_FILES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(plazza ${CORE_FILES})
target_link_libraries(plazza PRIVATE plazzaInclude)
target_compile_options(plazza PRIVATE -Wall -Wextra -Werror)

set_target_properties(plazza PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

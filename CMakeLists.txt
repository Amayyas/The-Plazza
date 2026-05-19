cmake_minimum_required(VERSION 3.16)
project(ThePlazza CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Werror)

add_executable(plazza 
    main.cpp
    src/Mutex.cpp
    src/ConditionVariable.cpp
    src/Thread.cpp
    src/Process.cpp
    src/IPC.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(plazza PRIVATE Threads::Threads)

# Doxygen setup
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX NO)
    # Output documentation inside the "docs" folder in the project root
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs)
    
    # Creates a "make doc" target
    doxygen_add_docs(
        doc
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        COMMENT "Generate Doxygen documentation"
    )
endif()

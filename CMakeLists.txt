# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.8)

# Set the C++ standard to C++20 and make it mandatory
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Hot Reload for MSVC compilers if supported
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Define the project name
project("Diglot")

# Create directories in the build directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/pos_data)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test_data)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Data/books/)

# Add SQLite as a static library
add_library(SQLite STATIC ${CMAKE_SOURCE_DIR}/lib/SQLite/sqlite3.c)
target_include_directories(SQLite PUBLIC ${CMAKE_SOURCE_DIR}/lib/SQLite)
target_compile_definitions(SQLite PRIVATE SQLITE_STATICLIB)

# Link additional libraries for UNIX systems
if(UNIX)
    target_link_libraries(SQLite PUBLIC dl pthread) 
endif()

# Add TranslatorLib as a static library
add_library(TranslatorLib STATIC
    src/Translator/pos.cpp
    src/Translator/dictionary.cpp
    src/Translator/translator.cpp  
)
target_include_directories(TranslatorLib PUBLIC ${CMAKE_SOURCE_DIR}/src/Translator/include)

# Add Translator as an executable
add_executable(Translator src/Translator/main_translator.cpp)

# Additional configurations for UNIX systems
if(UNIX)
    # Find and include GTK for file operations on Linux
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    include_directories(${GTK3_INCLUDE_DIRS})
    target_link_libraries(TranslatorLib ${GTK3_LIBRARIES})
    
    # Define the compilation command with pkg-config
    set(compile_command g++ file_chooser.cpp -o file_chooser ${GTK3_CFLAGS_OTHER} ${GTK3_LDFLAGS})

    # Add file_chooser as an executable
    add_executable(file_chooser src/Utils/file_chooser.cpp)

    # Create a custom target to run the compilation
    add_custom_target(compile_file_chooser
                        COMMAND ${compile_command}
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        COMMENT "Compiling file_chooser.cpp"
    )

    target_link_libraries(file_chooser ${GTK3_LIBRARIES})
endif()

# Link libraries
target_link_libraries(TranslatorLib SQLite)
target_link_libraries(Translator TranslatorLib)

# Copy the dictionary file to the build directory
configure_file(${CMAKE_SOURCE_DIR}/src/Translator/dict.sqlite ${CMAKE_BINARY_DIR}/dict.sqlite COPYONLY)

## Add Catch2 testing suite

# add_subdirectory(lib/Catch2)
# file(GLOB TEST_SOURCES "tests/*/*.cpp")
# add_executable(tests ${TEST_SOURCES})
# target_link_libraries(tests PRIVATE Catch2::Catch2WithMain TranslatorLib)
# include(CTest)
# include(Catch)
# catch_discover_tests(tests)

# Include Ultralight 
include(cmake/App.cmake)

# Set source files for GUI
set(GUI_SOURCES 
    "src/GUI/MyApp.h"
    "src/GUI/MyApp.cpp"
    "src/GUI/main.cpp"
    "src/GUI/logging.cpp"
)

# Register an executable with the name Diglot
add_app("${GUI_SOURCES}")
target_link_libraries(Diglot TranslatorLib)


# Option for code coverage
option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

# Custom targets to copy data
add_custom_target(copy_test_data ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/test_data/
            ${CMAKE_BINARY_DIR}/test_data/
)

add_custom_target(copy_pos_data ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/pos_data/
            ${CMAKE_BINARY_DIR}/pos_data/
)

add_custom_target(copy_book_data ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/Data/books/
            ${CMAKE_BINARY_DIR}/Data/books/
)

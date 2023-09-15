# CMakeList.txt : CMake project for Diglot, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
# Check if the coverage option is enabled

project ("Diglot")


# Copy myfile.txt to the build directory

# Add source to this project's executable.
# add_executable (Diglot "Diglot.cpp" "Diglot.h")

add_library(SQLite STATIC 
    ${CMAKE_SOURCE_DIR}/lib/SQLite/sqlite3.c
)

target_include_directories(SQLite PUBLIC 
    ${CMAKE_SOURCE_DIR}/lib/SQLite
)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/pos_data)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test_data)

if(UNIX)
    target_link_libraries(SQLite PUBLIC dl pthread) 
endif()

target_compile_definitions(SQLite PRIVATE SQLITE_STATICLIB)

add_library(SubstituterLib STATIC
    src/Substituter/substituter.cpp
)

target_include_directories(SubstituterLib PUBLIC
    ${CMAKE_SOURCE_DIR}/src/Substituter/include
)

add_library(TranslatorLib STATIC
    src/Translator/pos.cpp
    src/Translator/dictionary.cpp
    src/Translator/translator.cpp  
)

target_include_directories(TranslatorLib PUBLIC
    ${CMAKE_SOURCE_DIR}/src/Translator/include
)

add_executable(Translator
    src/Translator/main_translator.cpp
)

add_executable(Substituter
    src/Substituter/main_substituter.cpp
)



if(UNIX)
    # download and include GTK so that files can be opened on linux
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    include_directories(${GTK3_INCLUDE_DIRS})
    target_link_libraries(TranslatorLib ${GTK3_LIBRARIES})
    
    # Define the compilation command with pkg-config
    set(COMPILE_COMMAND g++ file_chooser.cpp -o file_chooser ${GTK3_CFLAGS_OTHER} ${GTK3_LDFLAGS})

    add_executable(file_chooser src/Utils/file_chooser.cpp)

    # Create a custom target to run the compilation
    add_custom_target(  compile_file_chooser
                        COMMAND ${COMPILE_COMMAND}
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        COMMENT "Compiling file_chooser.cpp"
    )

    target_link_libraries(file_chooser ${GTK3_LIBRARIES})
endif()

target_link_libraries(TranslatorLib SQLite)
target_link_libraries(Translator TranslatorLib)


configure_file(${CMAKE_SOURCE_DIR}/src/Translator/dict.sqlite ${CMAKE_BINARY_DIR}/dict.sqlite COPYONLY)


# Add Catch2 testing suite
add_subdirectory(lib/Catch2)
file(GLOB TEST_SOURCES "tests/*/*.cpp")
add_executable(tests ${TEST_SOURCES})
# add_executable(tests tests/tests.cpp )
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain SubstituterLib TranslatorLib)

include(CTest)
include(Catch)
catch_discover_tests(tests)  # Automatically discover and register tests
# CATCH_DISCOVER_TESTS(tests)
#Only compile front end on windows for now
# if (CMAKE_SYSTEM_NAME MATCHES "Windows")
# include Ultralight 
include(cmake/App.cmake)

# set source files for GUI. 
set(GUI_SOURCES "src/GUI/MyApp.h"
            "src/GUI/MyApp.cpp"
            "src/GUI/main.cpp")
# Will register an exe with the name Diglot. 
ADD_APP("${GUI_SOURCES}")
target_link_libraries(Diglot TranslatorLib)
if (CMAKE_VERSION VERSION_GREATER 3.12)
set_property(TARGET Diglot PROPERTY CXX_STANDARD 20)
    endif()
# endif()


option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    #Add required flags (GCC & LLVM/Clang)
endif()

add_custom_target(copy_test_data ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/test_data/
            ${CMAKE_BINARY_DIR}/test_data/)

add_custom_target(copy_pos_data ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/pos_data/
            ${CMAKE_BINARY_DIR}/pos_data/)




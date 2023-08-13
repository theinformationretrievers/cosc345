# CMakeList.txt : CMake project for Diglot, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Diglot")

# Add source to this project's executable.
# add_executable (Diglot "Diglot.cpp" "Diglot.h")

add_library(SubstituterLib STATIC
    src/Substituter/substituter.cpp
)

target_include_directories(SubstituterLib PUBLIC
    ${CMAKE_SOURCE_DIR}/src/Substituter/include
)

add_executable(Substituter
    src/Substituter/main_substituter.cpp
)

add_executable(Translator
    src/Translator/main_translator.cpp
)
target_link_libraries(Translator SubstituterLib)


# Add Catch2 testing suite
add_subdirectory(lib/Catch2)
file(GLOB TEST_SOURCES "tests/*/*.cpp")
add_executable(tests ${TEST_SOURCES})
# add_executable(tests tests/tests.cpp )
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
include(CTest)
include(Catch)
catch_discover_tests(tests)  # Automatically discover and register tests
# CATCH_DISCOVER_TESTS(tests)

# include Ultralight 
include(cmake/App.cmake)

# set source files for GUI. 
set(GUI_SOURCES "src/GUI/MyApp.h"
            "src/GUI/MyApp.cpp"
            "src/GUI/main.cpp")
# Will register an exe with the name Diglot. 
ADD_APP("${GUI_SOURCES}")


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Diglot PROPERTY CXX_STANDARD 20)
endif()


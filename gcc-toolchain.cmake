execute_process(COMMAND which g++-13
                OUTPUT_VARIABLE GPP_PATH
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_COMPILER "${GPP_PATH}")

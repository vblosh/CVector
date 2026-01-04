enable_testing()

# C Test sources
set(TEST_SOURCES
    test_cvector.c
)

# Create C test executable
add_executable(test_cvector ${TEST_SOURCES})

# Link against the library
target_link_libraries(test_cvector cvector)

# Add the C test
add_test(NAME cvector_tests COMMAND test_cvector)

# C++ Test sources
set(CXX_TEST_SOURCES
    test_cvector_cpp.cpp
)

# Create C++ test executable
add_executable(test_cvector_cpp ${CXX_TEST_SOURCES})

# Link against the library
target_link_libraries(test_cvector_cpp cvector)

# Set C++ standard for test
set_target_properties(test_cvector_cpp PROPERTIES LANGUAGE CXX CXX_STANDARD 17)

# Add the C++ test
add_test(NAME cvector_cpp_tests COMMAND test_cvector_cpp)

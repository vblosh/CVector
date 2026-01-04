# Example programs

add_executable(example_basic basic_example.c)
target_link_libraries(example_basic cvector)

add_executable(example_strings strings_example.c)
target_link_libraries(example_strings cvector)

add_executable(example_structs structs_example.c)
target_link_libraries(example_structs cvector)

add_executable(example_cpp_interface cpp_interface_example.cpp)
target_link_libraries(example_cpp_interface cvector)
set_target_properties(example_cpp_interface PROPERTIES LANGUAGE CXX)

find_package(Qt6 REQUIRED COMPONENTS Test)

function(add_maze_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE
        Qt6::Test
        Qt6::Core
        maze_lib
    )
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_maze_test(test_generator)
add_maze_test(test_solver)

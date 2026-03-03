find_package(Qt6 REQUIRED COMPONENTS Test)

set(VALIDATOR_SRC utils/maze_validator.cpp)

function(add_maze_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE
        Qt6::Test
        Qt6::Core
        maze_lib
    )
    target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR})
    add_test(NAME ${name} COMMAND ${name})
endfunction()



# these two need the validator
foreach(name test_maze_validator test_generator)
    add_executable(${name} ${name}.cpp ${VALIDATOR_SRC})
    target_link_libraries(${name} PRIVATE Qt6::Test Qt6::Core maze_lib)
    target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR})
    add_test(NAME ${name} COMMAND ${name})
endforeach()

add_maze_test(test_solver)

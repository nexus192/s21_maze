cmake_minimum_required(VERSION 3.16)

project(s21_maze VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_COVERAGE "Build with coverage instrumentation" OFF)
if(BUILD_COVERAGE)
    add_compile_options(--coverage)
    add_link_options(--coverage)
    
    find_program(GCOVR gcovr)
    if(GCOVR)
        add_custom_target(coverage
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            COMMAND ${GCOVR} -r ${CMAKE_SOURCE_DIR}
                --filter '${CMAKE_SOURCE_DIR}/src/lib/service/generator/'
                --filter '${CMAKE_SOURCE_DIR}/src/lib/service/solver/'
                --html --html-details --html-theme github.dark-green
                -o coverage.html
            COMMAND open coverage.html
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Running tests and generating coverage report..."
            DEPENDS test_generator test_solver
        )
    else()
        message(WARNING "gcovr not found, coverage target disabled")
    endif()
endif()
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Concurrent)
find_package(Qt6 REQUIRED COMPONENTS Svg)

qt_standard_project_setup(REQUIRES 6.5)

# create library for testability
add_library(maze_lib STATIC
    src/lib/service/generator/generator.cpp
    src/lib/service/ioParser/asyncIOParser.cpp
    src/lib/service/solver/solver.cpp
    src/lib/model/maze.cpp
)

target_include_directories(maze_lib PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(maze_lib PUBLIC Qt6::Core Qt6::Concurrent Qt6::Svg)

qt_add_resources(APP_RESOURCES
    src/app/resources/resources.qrc
)

qt_add_executable(apps21_maze
    src/app/main.cpp
    ${APP_RESOURCES}
)

qt_add_qml_module(apps21_maze
    URI s21_maze
    QML_FILES
        src/app/Main.qml
        src/app/windows/StartWindow.qml
        src/app/windows/MazeWindow.qml
        src/app/utils/TextButton.qml
        src/app/utils/MazeWidget.qml
        src/app/utils/StatusDialog.qml
        QML_FILES src/app/utils/SelectRowColDialog.qml
)

target_link_libraries(apps21_maze
    PRIVATE Qt6::Quick
    Qt6::Widgets
    maze_lib
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(apps21_maze PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apps21_maze
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)


# tests
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

include(GNUInstallDirs)
install(TARGETS apps21_maze
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

option(BUILD_COVERAGE "Build with coverage instrumentation" OFF)

if(BUILD_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

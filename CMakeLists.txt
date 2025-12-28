cmake_minimum_required(VERSION 3.16)

project(s21_maze VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Concurrent)

qt_standard_project_setup(REQUIRES 6.5)

# create library for testability
add_library(maze_lib STATIC
    src/lib/service/generator/generator.cpp
    src/lib/service/ioParser/asyncIOParser.cpp
    src/lib/service/solver/solver.cpp
    src/lib/model/maze.cpp
)

target_include_directories(maze_lib PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(maze_lib PUBLIC Qt6::Core Qt6::Concurrent)


qt_add_executable(apps21_maze
    src/app/ui-gui/main.cpp
)

qt_add_qml_module(apps21_maze
    URI s21_maze
    QML_FILES
        src/app/ui-gui/Main.qml
        src/app/ui-gui/windows/StartWindow.qml
        src/app/ui-gui/windows/MazeWindow.qml
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
    enable_testing()  # add this BEFORE add_subdirectory
    add_subdirectory(tests)
endif()

include(GNUInstallDirs)
install(TARGETS apps21_maze
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

cmake_minimum_required(VERSION 3.16)

project(s21_maze VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(apps21_maze
    src/app/ui-gui/main.cpp

    src/lib/service/generator/generator.cpp
    src/lib/service/ioParser/asyncIOParser.cpp
    src/lib/service/ioParser/asyncIOParser.cpp
    src/lib/service/solver/solver.cpp
    src/lib/model/maze.cpp
    src/lib/service/generator/generator.h
    src/lib/service/ioParser/asyncIOParser.h
    src/lib/service/solver/solver.h
    src/lib/model/maze.h
)

qt_add_qml_module(apps21_maze
    URI s21_maze
    QML_FILES
        src/app/ui-gui/Main.qml
        src/app/ui-gui/windows/StartWindow.qml
        src/app/ui-gui/windows/MazeWindow.qml
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

target_link_libraries(apps21_maze
    PRIVATE Qt6::Quick
)
target_link_libraries(apps21_maze PRIVATE Qt6::Core)
target_link_libraries(apps21_maze PRIVATE Qt6::Widgets)

include(GNUInstallDirs)
install(TARGETS apps21_maze
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

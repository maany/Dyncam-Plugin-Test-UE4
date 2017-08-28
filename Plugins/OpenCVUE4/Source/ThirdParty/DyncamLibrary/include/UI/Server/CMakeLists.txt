
# QT
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Widgets REQUIRED)

add_executable(ServerGui
        serverwindow.cpp
        serverwindow.h
        qtopencvwidget.cpp
        qtopencvwidget.h
        pointswidget.cpp
        pointswidget.h
        ServerGui.cpp)

set_property(TARGET ServerGui PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ServerGui PROPERTY CXX_STANDARD 14)

# so react uses "slots" and for qt slots is a keyword
# this way we need to use the following keywords in qt but can use react
# signals -> Q_SIGNALS
# slots -> Q_SLOTS
# emit -> Q_EMIT
# foreach -> Q_FOREACH
target_compile_definitions(ServerGui PRIVATE -DQT_NO_KEYWORDS)

target_include_directories(ServerGui PRIVATE
        ${PROJECT_SOURCE_DIR}/deps/cpp.react/include
        )
target_include_directories(ServerGui PRIVATE ${PROJECT_SOURCE_DIR}/)

target_link_libraries(ServerGui
        dyncam
        Qt5::Widgets)
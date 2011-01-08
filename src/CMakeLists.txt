add_definitions(-W -Wall -ansi -pedantic -g)
add_definitions(-Wno-unused-parameter -Wno-long-long)
add_definitions(-DCMAKE_BUILD)

include_directories(.)

aux_source_directory(log LOG_SOURCES)
aux_source_directory(math MATH_SOURCES)
aux_source_directory(misc MISC_SOURCES)
aux_source_directory(opengl OPENGL_SOURCES)
aux_source_directory(qtgui QTGUI_SOURCES)

set(SOURCES
    ${LOG_SOURCES}
    ${MATH_SOURCES}
    ${MISC_SOURCES}
    ${OPENGL_SOURCES}
    ${QTGUI_SOURCES})

# Boost stuff
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS
    date_time filesystem serialization system thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Qt 4 stuff
find_package(Qt4 COMPONENTS
    QtCore QtGui QtOpenGL REQUIRED)
include(${QT_USE_FILE})
QT4_AUTOMOC(${SOURCES})

# Executables
add_executable(qtgui/project
    ${SOURCES})

target_link_libraries(qtgui/project
    ${Boost_LIBRARIES} ${QT_LIBRARIES} GLU)

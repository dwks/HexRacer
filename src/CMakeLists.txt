add_definitions(-W -Wall -ansi -pedantic -g)
add_definitions(-Wno-unused-parameter -Wno-long-long)
add_definitions(-DCMAKE_BUILD)

include_directories(.)

include_directories(../build)  # for the moc

aux_source_directory(connection CONNECTION_SOURCES)
aux_source_directory(event EVENT_SOURCES)
aux_source_directory(log LOG_SOURCES)
aux_source_directory(math MATH_SOURCES)
aux_source_directory(misc MISC_SOURCES)
aux_source_directory(network NETWORK_SOURCES)
aux_source_directory(opengl OPENGL_SOURCES)
aux_source_directory(qtgui QTGUI_SOURCES)
aux_source_directory(server SERVER_DIR_SOURCES)
aux_source_directory(sdl SDL_DIR_SOURCES)

set(SOURCES
    ${LOG_SOURCES}
    ${MATH_SOURCES}
    ${MISC_SOURCES})

set(QT_SOURCES
    ${OPENGL_SOURCES}
    ${QTGUI_SOURCES})

set(SDL_SOURCES
    ${CONNECTION_SOURCES}
    ${NETWORK_SOURCES}
    ${EVENT_SOURCES}
    ${SDL_DIR_SOURCES})

set(SERVER_SOURCES
    ${CONNECTION_SOURCES}
    ${NETWORK_SOURCES}
    ${EVENT_SOURCES}
    ${SERVER_DIR_SOURCES})

# Boost stuff
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS
    date_time filesystem serialization system thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# OpenGL stuff
set(OPENGL_LIBRARIES GLU)

# SDL stuff
find_package(SDL)
include_directories(${SDL_INCLUDE_DIR})

# Qt 4 stuff
find_package(Qt4 COMPONENTS
    QtCore QtGui QtOpenGL REQUIRED)
include(${QT_USE_FILE})
QT4_AUTOMOC(${SOURCES} ${QT_SOURCES})

# The main project-core library
set(CORE_LIBRARY project-core)
add_library(${CORE_LIBRARY} ${SOURCES})

# Executables
add_executable(project
    ${QT_SOURCES})
set(OPENGL_HACK ${OPENGL_SOURCES})
list(REMOVE_ITEM OPENGL_HACK opengl/MainDisplay.cpp)
add_executable(sdl
    ${SDL_SOURCES} ${OPENGL_HACK})
add_executable(server
    ${SERVER_SOURCES})

target_link_libraries(project
    ${CORE_LIBRARY} ${Boost_LIBRARIES} ${QT_LIBRARIES} ${OPENGL_LIBRARIES})
target_link_libraries(sdl
    ${CORE_LIBRARY} ${Boost_LIBRARIES} ${SDL_LIBRARY} ${OPENGL_LIBRARIES})
target_link_libraries(server
    ${CORE_LIBRARY} ${Boost_LIBRARIES} ${OPENGL_LIBRARIES})

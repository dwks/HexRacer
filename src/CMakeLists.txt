add_definitions(-W -Wall -ansi -pedantic -g)
add_definitions(-Wno-unused-parameter -Wno-long-long)
add_definitions(-DCMAKE_BUILD)

include_directories(.)

include_directories(../build)  # for the moc
include_directories(../bullet-2.77/src)
link_directories(
    ../bullet-2.77/src/LinearMath
    ../bullet-2.77/src/BulletCollision
    ../bullet-2.77/src/BulletDynamics)

aux_source_directory(connection CONNECTION_SOURCES)
aux_source_directory(event EVENT_SOURCES)
aux_source_directory(log LOG_SOURCES)
aux_source_directory(math MATH_SOURCES)
aux_source_directory(misc MISC_SOURCES)
aux_source_directory(network NETWORK_SOURCES)
aux_source_directory(object OBJECT_SOURCES)
aux_source_directory(opengl OPENGL_SOURCES)
aux_source_directory(physics PHYSICS_SOURCES)
aux_source_directory(server SERVER_DIR_SOURCES)
aux_source_directory(sdl SDL_DIR_SOURCES)

set(SOURCES
    ${LOG_SOURCES}
    ${MATH_SOURCES}
    ${MISC_SOURCES}
    ${EVENT_SOURCES}
    ${OBJECT_SOURCES}
    ${CONNECTION_SOURCES}
    ${NETWORK_SOURCES})

set(SDL_SOURCES
    ${OPENGL_SOURCES}
    ${SDL_DIR_SOURCES}
    ${PHYSICS_SOURCES})

set(SERVER_SOURCES
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
add_executable(sdl
    ${SDL_SOURCES})
add_executable(server
    ${SERVER_SOURCES})

target_link_libraries(sdl
    ${CORE_LIBRARY} ${Boost_LIBRARIES} ${SDL_LIBRARY} ${OPENGL_LIBRARIES}
    LinearMath BulletCollision BulletDynamics)
target_link_libraries(server
    ${CORE_LIBRARY} ${Boost_LIBRARIES} ${OPENGL_LIBRARIES})

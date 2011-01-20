add_definitions(-W -Wall -ansi -pedantic -g)
add_definitions(-Wno-unused-parameter -Wno-long-long)
add_definitions(-DCMAKE_BUILD)

include_directories(.)

aux_source_directory(connection CONNECTION_SOURCES)
aux_source_directory(event EVENT_SOURCES)
aux_source_directory(log LOG_SOURCES)
aux_source_directory(math MATH_SOURCES)
aux_source_directory(misc MISC_SOURCES)
aux_source_directory(network NETWORK_SOURCES)
aux_source_directory(object OBJECT_SOURCES)
aux_source_directory(opengl OPENGL_SOURCES)
aux_source_directory(physics PHYSICS_SOURCES)
aux_source_directory(render RENDER_SOURCES)
aux_source_directory(server SERVER_DIR_SOURCES)
aux_source_directory(settings SETTINGS_SOURCES)
aux_source_directory(sdl SDL_DIR_SOURCES)

set(SOURCES
    ${LOG_SOURCES}
    ${MATH_SOURCES}
    ${MISC_SOURCES}
    ${EVENT_SOURCES}
    ${OBJECT_SOURCES}
    ${CONNECTION_SOURCES}
    ${NETWORK_SOURCES}
    ${PHYSICS_SOURCES}
    ${OPENGL_SOURCES}
    ${SETTINGS_SOURCES})

set(SDL_SOURCES
    ${SDL_DIR_SOURCES}
    ${RENDER_SOURCES})

set(SERVER_SOURCES
    ${SERVER_DIR_SOURCES})

# Boost stuff
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS
    date_time filesystem serialization system thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# OpenGL stuff
set(OPENGL_LIBRARIES GL GLU GLEW)

# SDL stuff
find_package(SDL)
find_package(SDL_image)
include_directories(${SDL_INCLUDE_DIR} ${SDLIMAGE_INCLUDE_DIR})

# Qt 4 stuff
#find_package(Qt4 COMPONENTS
#    QtCore QtGui QtOpenGL REQUIRED)
#include(${QT_USE_FILE})
#QT4_AUTOMOC(${SOURCES} ${QT_SOURCES})
#include_directories(../build)  # for the moc

# bullet stuff
set(BULLET_LIBRARIES BulletDynamics BulletCollision LinearMath)
include_directories(../bullet-2.77/src)
link_directories(
    ../bullet-2.77/src/LinearMath
    ../bullet-2.77/src/BulletCollision
    ../bullet-2.77/src/BulletDynamics)

# The main project-core library
set(CORE_LIBRARY project-core)
add_library(${CORE_LIBRARY} ${SOURCES})

# Executables
add_executable(sdl ${SDL_SOURCES})
add_executable(server ${SERVER_SOURCES})

set(COMMON_LIBRARIES
    ${CORE_LIBRARY} ${Boost_LIBRARIES} ${OPENGL_LIBRARIES} ${BULLET_LIBRARIES})

#target_link_libraries(sdl ${COMMON_LIBRARIES} ${SDL_LIBRARIES})
#target_link_libraries(server ${COMMON_LIBRARIES})

target_link_libraries(sdl
    ${CORE_LIBRARY} ${Boost_LIBRARIES} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${OPENGL_LIBRARIES}
    BulletDynamics BulletCollision LinearMath)
target_link_libraries(server
    ${CORE_LIBRARY} ${Boost_LIBRARIES} ${OPENGL_LIBRARIES}
    BulletDynamics BulletCollision LinearMath)


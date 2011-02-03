add_definitions(-W -Wall -ansi -pedantic -g)
add_definitions(-Wno-unused-parameter -Wno-long-long)
add_definitions(-DCMAKE_BUILD)

include_directories(.)

include_directories(../build)  # for the moc

aux_source_directory(connection CONNECTION_SOURCES)
aux_source_directory(event EVENT_SOURCES)
aux_source_directory(handler HANDLER_SOURCES)
aux_source_directory(log LOG_SOURCES)
aux_source_directory(map MAP_SOURCES)
aux_source_directory(mapeditor MAP_EDITOR_DIR_SOURCES)
aux_source_directory(math MATH_SOURCES)
aux_source_directory(misc MISC_SOURCES)
aux_source_directory(network NETWORK_SOURCES)
aux_source_directory(object OBJECT_SOURCES)
aux_source_directory(opengl OPENGL_SOURCES)
aux_source_directory(paint PAINT_SOURCES)
aux_source_directory(physics PHYSICS_SOURCES)
aux_source_directory(render RENDER_SOURCES)
aux_source_directory(server SERVER_DIR_SOURCES)
aux_source_directory(settings SETTINGS_SOURCES)
aux_source_directory(sdl SDL_DIR_SOURCES)
aux_source_directory(sound SOUND_SOURCES)
aux_source_directory(timing TIMING_SOURCES)
aux_source_directory(widget WIDGET_SOURCES)

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
    ${SETTINGS_SOURCES}
    ${RENDER_SOURCES}
    ${PAINT_SOURCES}
    ${TIMING_SOURCES}
    ${HANDLER_SOURCES}
    ${MAP_SOURCES}
    ${WIDGET_SOURCES})

set(SDL_SOURCES
    ${SDL_DIR_SOURCES}
    ${SOUND_SOURCES})

set(SERVER_SOURCES
    ${SERVER_DIR_SOURCES})

set(MAP_EDITOR_SOURCES
    ${MAP_EDITOR_DIR_SOURCES})

# Boost stuff
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS
    date_time filesystem serialization system thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# OpenGL stuff
set(OPENGL_LIBRARIES GL GLU GLEW)

# OpenAL
find_package(OpenAL)

# Qt 4 stuff
find_package(Qt4 COMPONENTS
    QtCore QtGui QtOpenGL REQUIRED)
include(${QT_USE_FILE})
QT4_AUTOMOC(${MAP_EDITOR_SOURCES})

# SDL stuff
find_package(SDL)
find_package(SDL_image)
include_directories(${SDL_INCLUDE_DIR} ${SDLIMAGE_INCLUDE_DIR})

# bullet stuff
set(BULLET_PATH ../bullet-2.77)
set(BULLET_LIBRARIES BulletDynamics BulletCollision LinearMath)
include_directories(${BULLET_PATH}/src)
link_directories(
    ${BULLET_PATH}/src/LinearMath
    ${BULLET_PATH}/src/BulletCollision
    ${BULLET_PATH}/src/BulletDynamics)

# The main project-core library
set(CORE_LIBRARY project-core)
add_library(${CORE_LIBRARY} ${SOURCES})

# Executables
add_executable(sdl ${SDL_SOURCES})
add_executable(server ${SERVER_SOURCES})
add_executable(editor ${MAP_EDITOR_SOURCES})

set(COMMON_LIBRARIES
    ${CORE_LIBRARY}
    ${Boost_LIBRARIES}
    ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${BULLET_LIBRARIES})

target_link_libraries(sdl ${COMMON_LIBRARIES} ${OPENAL_LIBRARY} alut)
target_link_libraries(server ${COMMON_LIBRARIES})
target_link_libraries(editor ${COMMON_LIBRARIES} QtCore QtGui QtOpenGL)

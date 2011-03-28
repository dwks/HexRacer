# Enable warnings
add_definitions(-W -Wall -ansi -pedantic -g)
add_definitions(-Wno-unused-parameter -Wno-long-long)
add_definitions(-DCMAKE_BUILD)

# Some useful include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(../build)  # for the moc


################
#### Sources

# List of all source directories, in alphabetical order
aux_source_directory(ai AI_SOURCES)
aux_source_directory(connection CONNECTION_SOURCES)
aux_source_directory(event EVENT_SOURCES)
aux_source_directory(gui GUI_SOURCES)
aux_source_directory(handler HANDLER_SOURCES)
aux_source_directory(history HISTORY_SOURCES)
aux_source_directory(hud HUD_SOURCES)
aux_source_directory(input INPUT_SOURCES)
aux_source_directory(log LOG_SOURCES)
aux_source_directory(map MAP_SOURCES)
aux_source_directory(mapeditor MAP_EDITOR_DIR_SOURCES)
aux_source_directory(math MATH_SOURCES)
aux_source_directory(mesh MESH_SOURCES)
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
aux_source_directory(shader SHADER_SOURCES)
aux_source_directory(sound SOUND_SOURCES)
aux_source_directory(timing TIMING_SOURCES)
aux_source_directory(widget WIDGET_SOURCES)
aux_source_directory(world WORLD_SOURCES)

# Source groups, for e.g. Visual Studio
source_group("ai" FILES ${AI_SOURCES})
source_group("connection" FILES ${CONNECTION_SOURCES})
source_group("event" FILES ${EVENT_SOURCES})
source_group("gui" FILES ${GUI_SOURCES})
source_group("handler" FILES ${HANDLER_SOURCES})
source_group("history" FILES ${HISTORY_SOURCES})
source_group("hud" FILES ${HUD_SOURCES})
source_group("input" FILES ${INPUT_SOURCES})
source_group("log" FILES ${LOG_SOURCES})
source_group("map" FILES ${MAP_SOURCES})
source_group("map_editor" FILES ${MAP_EDITOR_DIR_SOURCES})
source_group("math" FILES ${MATH_SOURCES})
source_group("mesh" FILES ${MESH_SOURCES})
source_group("misc" FILES ${MISC_SOURCES})
source_group("network" FILES ${NETWORK_SOURCES})
source_group("object" FILES ${OBJECT_SOURCES})
source_group("opengl" FILES ${OPENGL_SOURCES})
source_group("paint" FILES ${PAINT_SOURCES})
source_group("physics" FILES ${PHYSICS_SOURCES})
source_group("render" FILES ${RENDER_SOURCES})
source_group("server" FILES ${SERVER_DIR_SOURCES})
source_group("settings" FILES ${SETTINGS_SOURCES})
source_group("sdl_dir" FILES ${SDL_DIR_SOURCES})
source_group("shader" FILES ${SHADER_SOURCES})
source_group("sound" FILES ${SOUND_SOURCES})
source_group("timing" FILES ${TIMING_SOURCES})
source_group("widget" FILES ${WIDGET_SOURCES})
source_group("world" FILES ${WORLD_SOURCES})

# Sources for the shared library
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
    ${SHADER_SOURCES}
    ${PAINT_SOURCES}
    ${TIMING_SOURCES}
    ${HANDLER_SOURCES}
    ${MAP_SOURCES}
    ${MESH_SOURCES}
    ${WORLD_SOURCES})

# Sources for the client
set(SDL_SOURCES
    ${SDL_DIR_SOURCES}
    ${SOUND_SOURCES}
    ${WIDGET_SOURCES}
    ${GUI_SOURCES}
    ${HUD_SOURCES}
    ${HISTORY_SOURCES}
    ${INPUT_SOURCES})

# Sources for the server
set(SERVER_SOURCES
    ${SERVER_DIR_SOURCES}
    ${AI_SOURCES})

# Sources for the map editor
set(MAP_EDITOR_SOURCES
    ${MAP_EDITOR_DIR_SOURCES})


################
#### Libraries

# Boost
#find_package(Boost)
find_package(Boost COMPONENTS
    serialization system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# OpenGL
find_package(OpenGL)
include_directories(${OPENGL_INCLUDE_DIR})

# GLEW
set(GLEW_LIBRARY GLEW)

# OpenAL
find_package(OpenAL)
include_directories(${OPENAL_INCLUDE_DIR})

# SDL
find_package(SDL)
find_package(SDL_image)
find_package(SDL_ttf)
include_directories(
    ${SDL_INCLUDE_DIR}
    ${SDLIMAGE_INCLUDE_DIR}
    ${SDLTTF_INCLUDE_DIR})

# bullet
set(BULLET_ROOT ../bullet-2.77)

include_directories(${BULLET_ROOT}/src)
set(BULLET_LIBRARIES BulletDynamics BulletCollision LinearMath)
link_directories(
    ${BULLET_ROOT}/src/LinearMath
    ${BULLET_ROOT}/src/BulletCollision
    ${BULLET_ROOT}/src/BulletDynamics)

# Qt 4 stuff
find_package(Qt4 COMPONENTS
    QtCore QtGui QtOpenGL REQUIRED)
if(QT_FOUND)
    include(${QT_USE_FILE})
    QT4_AUTOMOC(${MAP_EDITOR_SOURCES})
endif(QT_FOUND)


################
#### Executables

# The main project-core library
set(CORE_LIBRARY project-core)
add_library(${CORE_LIBRARY} ${SOURCES})

# Libraries to link with
set(COMMON_LIBRARIES
    ${CORE_LIBRARY}
    ${Boost_LIBRARIES}
    ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${BULLET_LIBRARIES})

# Executables
add_executable(sdl ${SDL_SOURCES})
add_executable(server ${SERVER_SOURCES})
add_executable(editor ${MAP_EDITOR_SOURCES})

target_link_libraries(sdl ${COMMON_LIBRARIES} ${SDLTTF_LIBRARY} ${OPENAL_LIBRARY} alut)
target_link_libraries(server ${COMMON_LIBRARIES})

if(QT_FOUND)
    target_link_libraries(editor ${COMMON_LIBRARIES} QtCore QtGui QtOpenGL)
endif(QT_FOUND)

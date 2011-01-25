README for HexRacers
====================


Using the game
--------------

The game can be launched standalone by running "sdl"; or if the "server" is
run first then any "sdl" clients launched will connect to the server.

Note: sdl.exe and server.exe are Windows executables,
      sdl and server are Linux ones.

Controls:
- Move the camera with a joystick (which must be plugged in before the game
  runs), or with the left mouse button and the keys 'w', 'a', 's', and 'd'.
- Drive the car with the arrow keys (add levitation with the space key)
- Lay down paint behind the car with 'p'; erase existing paint with 'o'
  [You can also paint in front of the camera with the right mouse button,
  and erase with the middle mouse button.]
- Pressing ESCAPE (or clicking the X button) closes the game.

Settings (can be changed without recompiling in config.txt)
- Paint generation can be disabled by setting "render.paint.enabled = 0":
  this speeds up the game launch time
- The screen resolution can be changed (try 0, 0 for the entire screen)
  [Note that the game window is also resizable.]


Compiling the game
------------------

Libraries used:
- Simple DirectMedia Layer (SDL 1.2.14) and SDL_image
- Bullet (2.77)
- Boost (1.43)
- OpenAL [but this is currently disabled in config.h]

There are two Visual Studio project files that our team has been using to
compile this code; you may find them a useful starting point if you wish to
build our game from source.


Known bugs
----------

Our shaders can be set to high quality by setting "render.quality = 2" in
config.txt; while this generally works, on one of our test Windows systems it
did not. So this has been disabled. Please feel free to try this.

Joystick axis mapping has been configured for Linux and may not work under
Windows at this time.

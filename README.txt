README for HexRacers
====================

Status of the game
------------------

Some notes:

- While painting or erasing, you have a speed penalty applied.
- If you are traveling on your own paint, you get an appropriate speed burst.
  Also the colour of the vehicle's trim changes if you currently have a burst.
- When you fall off the track (or press 'h') you are warped to the nearest
  point on the track.
- The bar on the left shows the percentage you have completed of the track.
- Shadows were just implemented and may have a few bugs.
- Most of the GUI is unimplemented, don't be surprised when clicking on
  buttons results in nothing happening (except maybe debug console output).
- Networking is still jerky; it works but is not very playable at the moment.



Using the game
--------------

Launch the game by running "sdl". Then click on "Start game" to launch a
single player game, or "Join game" to attempt to join a server (this defaults
to a single-player game if no server is running). A server can be run by just
launching "server"; make sure the port specified in config.txt (1820 by
default) is not blocked by your firewall.

Note: sdl.exe and server.exe are Windows executables,
      sdl and server are Linux ones.

Controls:
- Drive the car with the arrow keys (add levitation with the space key), or
  with the two hats on a joystick (which must be plugged in before the game
  starts).
- Lay down paint behind the car with 'p'; erase existing paint with 'o'
  [You can also paint in front of the camera with the right mouse button,
  and erase with the middle mouse button.]
- Press ESCAPE (or click the menu button) to get to the game menu, where you
  can click on the Quit button. Also, the first letter of a button's label is
  often mapped as its shortcut.

Debug controls:
- Toggle the debug camera by pressing 'c'.
- Move the camera with the rightt mouse button and translate it with the keys
  'w', 'a', 's', and 'd'.
- Toggle debug drawing with the enter key.
- Toggle path node painting with the backslash key.

Settings (can be changed without recompiling in config.txt)
- The screen resolution can be changed (try 0, 0 for the entire screen)
  [Note that the game window is also resizable.]
  
 Changing the Map:
 Change the map by uncommenting (removing the #) the map setting and commenting the others in the config file.
 By default, the map is a very large version of a prototype map we created. The other two
 are smaller versions of it. testtrack2 uses darker lighting, and testtrack3 uses brigter lighting.


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

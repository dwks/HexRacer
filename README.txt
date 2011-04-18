=== README for HexRacer

--- Using the game

Launch the game by running "sdl". You can play a single player game or launch
a server automatically from the "Host game" screen.

Note: sdl.exe and server.exe are Windows executables,
      sdl and server are Linux ones.

Keyboard controls:
- Drive the car with the arrow keys.
- Lay down paint behind the car with 'p' or 'z'.
- Erase existing paint with 'o' or 'x'.
- You can often press the first letter of a button's label to activate it.

Joystick controls:
- Select the active joystick by clicking "Select joystick" in the Settings menu
  and press any button on the joystick. The joystick may work without this, but
  no guarantees. Also, the joystick must be plugged in before the game launches.
- The left stick turns and triggers accelerate and brake.
- The A button toggles painting, B toggles erasing.
- The menus cannot yet be controlled by the joystick, use the mouse instead.

Debug controls:
- Switch the camera to follow other players with '[' and ']'.
- Toggle the debug camera by pressing 'c'.
- Move the debug camera with the right mouse button and translate it with the
  keys 'w', 'a', 's', and 'd'.
- Toggle debug drawing with the enter key.
- Toggle path node painting with the backslash key.
- The space bar levitates the car.


--- Status of the game

All game mechanics are implemented.
- While painting or erasing, you have a speed penalty applied.
- Traveling on your team's paint gives you an appropriate speed burst.
- You paint the same colour as your team.

Other new features:
- There is a game lobby for multiplayer games.
- The hex list of players on the left shows the order of players in the race.
  Hexes become tinged with white when players finish.
- The game actually finishes now and displays players' times, with bonuses for
  placing first, second, etc.
- Network jerkiness should be better now.
- We have a new car model now.
- Some other animations added to the GUI.
- AI improved, they actually decide when to paint and erase and speed boost.
- Different game types (overwritable paint, etc).


--- Compiling the game

Libraries used:
- Simple DirectMedia Layer (SDL 1.2.14), with additional libraries:
    - SDL_image
    - SDL_ttf
- OpenGL and GLU and GLEW
- Bullet physics (2.77)
- Boost (1.42+), specifically:
    - Boost.Serialization, Boost.System, and Boost.Filesystem (require linking)
    - Boost.SmartPtr and Boost.DateTime (require headers only)
- OpenAL [can be disabled in config.h]
- Qt 4 (for the map editor only)

There are two Visual Studio project files that our team has been using to
compile this code; you may find them a useful starting point if you wish to
build our game from source.


--- Known bugs

On some older systems the shaders do not work correctly, or make the game too
slow. The setting "render.quality" can be changed in config.txt, or in the
Settings menu, a lower graphics quality can be set. (The default is "very high"
quality.)

The server should exit automatically now, but it may still be necessary to kill
it if the client crashes (it will automatically exit after 30 seconds in this
case). There is a client crash that only occurs when joining a game for the
second time. If this happens, restart the game before joining other games.

Note: practice mode is not intended to have AIs or multiplayer. It's just for
exploring maps.


<h1 align="center">
  <a href="https://logalex96.github.io/minesweeper/"><img src="https://raw.githubusercontent.com/logalex96/minesweeper/master/docs/logo.png" alt="Minesweeper Game" width="822"></a>
  <br>
</h1>

<h4 align="center"> :heart: A new old game that just arrived :heart: </h4>

<p align="center"><img src="https://raw.githubusercontent.com/logalex96/minesweeper/master/docs/game_demo.gif" alt="minesweeper game play demo"></p>

### Instructions for MineSweeper

**Controls**

- `Left-click` - reveals a block
- `Right-click` / `Space` - x1 place a flag
- `Right-click` / `Space` - x2 place a question mark
- `Middle-click` / `L-Shift` - reveal its adjacent squares.
- `F2` - reveal its adjacent squares.

**Quick Start:**

- You are presented with a board of squares. 
- Some squares contain mines (bombs), others don't. If you click on a square containing a bomb, you lose. If you manage to click all the squares (without clicking on any bombs) you win.
- Clicking a square which doesn't have a bomb reveals the number of neighbouring squares containing bombs. Use this information plus some guess work to avoid the bombs.
- To open a square, point at the square and click on it. To mark a square you think is a bomb, point and right-click (or hover with the mouse and press `Space`).

<p align="center"><img src="https://raw.githubusercontent.com/logalex96/minesweeper/master/docs/screenshots.png" alt="minesweeper game screenshots"></p>

**Detailed Instructions:**

- A squares "neighbours" are the squares adjacent above, below, left, right, and all 4 diagonals. Squares on the sides of the board or in a corner have fewer neighbors. The board does not wrap around the edges.
- If you open a square with 0 neighboring bombs, all its neighbors will automatically open. This can cause a large area to automatically open.
- To remove a bomb marker from a square, point at it and right-click again.
- The first square you open is never a bomb.
- If you mark a bomb incorrectly, you will have to correct the mistake before you can win. Incorrect bomb marking doesn't kill you, but it can lead to mistakes which do.
- You don't have to mark all the bombs to win; you just need to open all non-bomb squares.
- Right-clicking twice will give you a question mark symbol which can be useful if you are unsure about a square (or double press `space`)
- Click the MineSweeper happy face to start a new game.

**Status Information:**

- The upper left corner contains the number of bombs left to find. The number will update as you mark and unmark squares.
- The upper right corner contains a time counter. The timer will max out at 99 (1 minutes 39 seconds) if the game width it is less than 9 otherwise at 999 (16 minutes 39 seconds).

**Options and Enhancements:**

- Marks (?) - Right clicking on a marked bomb will change the flag into a question mark. Right clicking again will change it back into an unmarked square.
- Reveal adjacent squares function - Hover with the mouse and press `shift` or `middle click` to reveal all adjacent squares.

<p align="center">
<a href="http://www.sfml-dev.org/index.php"><img src="http://www.sfml-dev.org/images/logo.png" alt="SFML" width="252"></a>
</p>

### What is SFML?

SFML is a simple to use and portable API written in C++. You can think of it as an object oriented SDL. SFML is made of modules in order to be as useful as possible for everyone. You can use SFML as a minimalist window system in order to use OpenGL, or as a complete multimedia library full of features to build video games or multimedia softwares.

<p align="center">
######SFML Version used in game: SFML 2.4.1, GCC 4.9.2 TDM (SJLJ) - 32-bit
</p>

### What dependencies does SFML have?

SFML depends on a few other libraries, so before starting to compile you must have their development files installed.

On Windows and Mac OS X, all the needed dependencies are provided directly with SFML, so you don't have to download/install anything. Compilation will work out of the box.

On Linux however, nothing is provided and SFML relies on your own installation of the libraries it depends on. Here is a list of what you need to install before compiling SFML:

 - pthread
 - opengl
 - xlib
 - xcb
 - x11-xcb
 - xcb-randr
 - xcb-image
 - xrandr
 - udev
 - freetype
 - jpeg
 - openal
 - flac
 - vorbis
 
The exact name of the packages depend on each distribution. And don't forget to install the development version of these packages.

SFML has also internal dependencies: Audio and Window depend on System, while Graphics depends on System and Window. In order to use the Graphics module, you must link with Graphics, Window, and System (the order of linkage matters with GCC).


## License

2017. Copyright (c) [Loghin L Alexandru](https://github.com/logalex96).

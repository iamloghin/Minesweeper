<h1 align="center">
  <a href=""><img src="http://i.imgur.com/dCYHKmo.png" alt="Minesweeper Game" width="822"></a>
  <br>
</h1>

<h4 align="center"> :heart: A new old game that just arrived :heart: </h4>

<p align="center"><img src="https://raw.githubusercontent.com/logalex96/minesweeper/master/game_demo.gif?token=AF9Zh5SQo2CzDlEXvQA3eqgfM0ERyQP4ks5YgUBswA%3D%3D" alt="minesweeper game play demo"></p>

### Instructions for MineSweeper

**Quick Start:**

- You are presented with a board of squares. 
- Some squares contain mines (bombs), others don't. If you click on a square containing a bomb, you lose. If you manage to click all the squares (without clicking on any bombs) you win.
- Clicking a square which doesn't have a bomb reveals the number of neighbouring squares containing bombs. Use this information plus some guess work to avoid the bombs.
- To open a square, point at the square and click on it. To mark a square you think is a bomb, point and right-click (or hover with the mouse and press `Space`).

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

## License

2017. Copyright (c) [Loghin L Alexandru](https://github.com/logalex96).

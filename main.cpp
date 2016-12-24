#include "header.h"

int main() {
  srand(time(0));

  // user input
  std::cout << "Width = ";
  std::cin >> width;
  std::cout << "Height = ";
  std::cin >> height;
  std::cout << "Mine number = ";
  std::cin >> mineNumber;

  squareNumber = width * height;
  unrevealed = squareNumber;

  sf::RenderWindow appWindow(sf::VideoMode(width * squareSize, height * squareSize), "Minesweeper!");
  sf::Texture t;
  t.loadFromFile("images/tiles.png");
  sf::Sprite s(t);

  // first things first (create the board)
  for (int i = 0; i < squareNumber; i++) {
    board[i] = 0;
    boardRevealed[i] = 0;
  }

  placeMines();

  for (int i = 0; i < squareNumber; i++)
    countSurrounding(i % width, i / width);

  // Sounds & Music
  sf::SoundBuffer buffer_mleft, buffer_flag, buffer_unflag, buffer_gameOver, buffer_gameWin;
  sf::Sound sound_mleft, sound_flag, sound_unflag, sound_gameOver, sound_gameWin;
  sf::Music music;

  if (!buffer_mleft.loadFromFile("sounds/click.wav"))
    std::cout << "can't find the click.wav sound! :(" << std::endl;
  if (!buffer_flag.loadFromFile("sounds/flag.wav"))
    std::cout << "can't find the flag.wav sound! :(" << std::endl;
  if (!buffer_unflag.loadFromFile("sounds/unflag.wav"))
    std::cout << "can't find the unflag.wav sound! :(" << std::endl;
  if (!buffer_gameOver.loadFromFile("sounds/gameOver.wav"))
    std::cout << "can't find the gameOver.wav sound! :(" << std::endl;
  if (!buffer_gameWin.loadFromFile("sounds/gameWin.wav"))
    std::cout << "can't find the gameWin.wav sound! :(" << std::endl;
  if (!music.openFromFile("sounds/music.ogg"))
    std::cout << "can't find the music.ogg sound! :(" << std::endl;

  // Change some parameters
  music.setPitch(1); // increase the pitch
  music.setVolume(30); // reduce the volume
  music.setLoop(true); // make it loop

  sound_mleft.setBuffer(buffer_mleft);
  sound_flag.setBuffer(buffer_flag);
  sound_unflag.setBuffer(buffer_unflag);
  sound_gameOver.setBuffer(buffer_gameOver);
  sound_gameWin.setBuffer(buffer_gameWin);

  music.play();

  while (appWindow.isOpen()) {
    sf::Vector2i pos = sf::Mouse::getPosition(appWindow);
    int mouseX = pos.x / squareSize;
    int mouseY = pos.y / squareSize;

    sf::Event e;
    while (appWindow.pollEvent(e)) {
      if (e.type == sf::Event::Closed)
        appWindow.close();

      if (e.type == sf::Event::MouseButtonPressed && gameOver != 1) {
        if (e.key.code == sf::Mouse::Left) {
          if (boardRevealed[mouseY * width + mouseX] == 0) {
            if (board[mouseY * width + mouseX] == 9) {
              music.stop();
              sound_gameOver.play();
            } else sound_mleft.play();
            reveal(mouseX, mouseY);
          }
        } else if (e.key.code == sf::Mouse::Right) {
          if (boardRevealed[mouseY * width + mouseX] == 2) {
            sound_unflag.play();
            boardRevealed[mouseY * width + mouseX] = 0;
          } else if (boardRevealed[mouseY * width + mouseX] == 0) {
            sound_flag.play();
            boardRevealed[mouseY * width + mouseX] = 2;
          }
        }
      }
    }

    appWindow.clear(Background_color);

    for (int i = 0; i < squareNumber; i++) {
      if (boardRevealed[i] == 1)
        s.setTextureRect(sf::IntRect(board[i] * squareSize, 0, squareSize, squareSize));
      else if (boardRevealed[i] == 0)
        s.setTextureRect(sf::IntRect(10 * squareSize, 0, squareSize, squareSize));
      else if (boardRevealed[i] == 2)
        s.setTextureRect(sf::IntRect(11 * squareSize, 0, squareSize, squareSize));

      // show all not found Mines when gameOver = true
      if (board[i] == 9 && gameOver && boardRevealed[i] != 2)
        s.setTextureRect(sf::IntRect(9 * squareSize, 0, squareSize, squareSize));

      s.setPosition(i % width * squareSize, i / width * squareSize);
      appWindow.draw(s);
    }

    // Add THE END text //
    sf::Text text;
    sf::Font font;
    font.loadFromFile("kongtext.ttf");
    text.setFont(font);
    text.setCharacterSize(20);
    text.setStyle(sf::Text::Bold);
    text.setColor(sf::Color::White);

    // if won //
    if (unrevealed == mineNumber) {
      text.setString("YOU WIN");
      text.setPosition(width / 3.5 * squareSize, height / 2.5 * squareSize);
      appWindow.clear(sf::Color::Green);
      appWindow.draw(text);
      if (won == false) {
        won = true;
        music.stop();
        sound_gameWin.play();
      }
    }
    // else if lose //
    else if (gameOver == 1) {
      text.setString("YOU LOSE");
      text.setPosition(width / 3.5 * squareSize, height / 2.5 * squareSize);
      sf::Texture b;
      b.loadFromFile("images/purple.png");
      b.setRepeated(true);
      sf::Sprite s(b);
      sf::Sprite sprite;
      sprite.setTexture(b);
      sprite.setTextureRect(sf::IntRect(0, 0, (width + 2) * squareSize, (height + 2) * squareSize));
      sprite.setColor(sf::Color(255, 255, 255, 200));
      appWindow.draw(sprite);

      appWindow.draw(text);
    }
    appWindow.display();
  }
  return 0;
}

int nearMines(int x, int y) {
  if (x >= 0 && x < width && y >= 0 && y <= height) {
    if (board[y * width + x] == 9) return 1;
    else return 0;
  } else return 0;
}

void countSurrounding(int x, int y) {
  if (board[y * width + x] == 9) return;
  int result = 0;
  if (nearMines(x + 1, y)) result++;
  if (nearMines(x - 1, y)) result++;
  if (nearMines(x, y - 1)) result++;
  if (nearMines(x, y + 1)) result++;
  if (nearMines(x + 1, y + 1)) result++;
  if (nearMines(x + 1, y - 1)) result++;
  if (nearMines(x - 1, y + 1)) result++;
  if (nearMines(x - 1, y - 1)) result++;
  board[y * width + x] = result;
}

void placeMines() {
  int count = 0;
  while (count < mineNumber) {
    int randPosition = rand() % squareNumber;
    if (board[randPosition] != 9) {
      board[randPosition] = 9;
      ++count;
    }
  }
}

bool revealed(int x, int y) {
  if (x < 0 || x >= width || y < 0 || y >= height) return true;
  if (boardRevealed[y * width + x] != 0) return true;
  return false;
}

void display(int x, int y, int firstX, int firstY) {
  if (x < 0 || x >= width || y < 0 || y >= height) return;
  int i = y * width + x;
  if (boardRevealed[i] != 0 || board[i] == 9) return;
  boardRevealed[i] = 1;
  --unrevealed;
  if (board[i] == 0) {
    if (!revealed(x + 1, y)) {
      display(x + 1, y, x, y);
    }
    if (!revealed(x - 1, y)) {
      display(x - 1, y, x, y);
    }
    if (!revealed(x, y + 1)) {
      display(x, y + 1, x, y);
    }
    if (!revealed(x, y - 1)) {
      display(x, y - 1, x, y);
    }
    if (!revealed(x + 1, y + 1)) {
      display(x + 1, y + 1, x, y);
    }
    if (!revealed(x + 1, y - 1)) {
      display(x + 1, y - 1, x, y);
    }
    if (!revealed(x - 1, y + 1)) {
      display(x - 1, y + 1, x, y);
    }
    if (!revealed(x - 1, y - 1)) {
      display(x - 1, y - 1, x, y);
    }
  }
}

void reveal(int x, int y) {
  if (x < 0 || x >= width || y < 0 || y >= height) return;
  int i = y * width + x;
  if (boardRevealed[i] == 0) {
    boardRevealed[i] = 1;
    --unrevealed;
    if (board[i] == 0) {
      display(x + 1, y, x, y);
      display(x - 1, y, x, y);
      display(x, y + 1, x, y);
      display(x, y - 1, x, y);
      display(x + 1, y - 1, x, y);
      display(x + 1, y + 1, x, y);
      display(x - 1, y - 1, x, y);
      display(x - 1, y + 1, x, y);
    }
    if (board[i] == 9) {
      gameOver = true;
    }
  }
}

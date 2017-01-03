#include "header.h"

int main()
{
    srand(time(0));

// user input
    std::cout << "Width = ";
    std::cin >> width;
    std::cout << "Height = ";
    std::cin >> height;
    std::cout << "Mine number = ";
    std::cin >> mineNumber;

    minesLeft = mineNumber;
    squareNumber = width * height;
    unrevealed = squareNumber;

    // Create the window of the application //
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow appWindow(sf::VideoMode((width+gameBorder_right+gameBorder_left)*squareSize,(height+gameBorder_top+gameBorder_bottom)*squareSize, desktop.bitsPerPixel), "Minesweeper!");

    // Sounds & Music
    sf::SoundBuffer buffer_mleft, buffer_flag, buffer_unflag, buffer_gameOver, buffer_gameWin;
    sf::Sound sound_mleft, sound_flag, sound_unflag, sound_gameOver, sound_gameWin;
    sf::Music music;

    if (!buffer_mleft.loadFromFile("sounds/click.wav")			||
            !buffer_flag.loadFromFile("sounds/flag.wav")			||
            !buffer_unflag.loadFromFile("sounds/unflag.wav")		||
            !buffer_gameOver.loadFromFile("sounds/gameOver.wav")	||
            !buffer_gameWin.loadFromFile("sounds/gameWin.wav")		||
            !music.openFromFile("sounds/music.ogg"))
    {
        //std::cout << "can't find something in sounds :(" << std::endl;
    }

    // Change some parameters
    music.setPitch(1); // increase the pitch
    music.setVolume(30); // reduce the volume
    music.setLoop(true); // make it loop

    sound_mleft.setBuffer(buffer_mleft);
    sound_flag.setBuffer(buffer_flag);
    sound_unflag.setBuffer(buffer_unflag);
    sound_gameOver.setBuffer(buffer_gameOver);
    sound_gameWin.setBuffer(buffer_gameWin);

    // Load the textures used in the game
    sf::Texture image_sprite, Background_image, faces;
    if(!image_sprite.loadFromFile("images/tiles.png")   ||
            !faces.loadFromFile("images/faces.png")          ||
            !Background_image.loadFromFile("images/tiles.png", sf::IntRect(15 * squareSize, 0, squareSize, squareSize)))
    {
        std::cout << "can't find all images :(" << std::endl;
    }
    sf::Sprite s(image_sprite), background_sprite(Background_image), f(faces);;
    Background_image.setRepeated(true);
    background_sprite.setTextureRect(sf::IntRect(0,0,desktop.width,desktop.height));
    f.setPosition(appWindow.getDefaultView().getSize().x / 2 - squareSize/2, squareSize);

    // Load the text font //
    sf::Font font;
    if (!font.loadFromFile("fonts/visitor1.ttf"))
        std::cout << "can't find the font (visitor1.ttf) :(" << std::endl;

    // Initialize the score text //
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setScale(2.f, 2.f);

    // let's play the music //
    music.play();

    // Main loop //
    while (appWindow.isOpen())
    {
        sf::Vector2i pos = sf::Mouse::getPosition(appWindow);
        int mouseX = pos.x / squareSize - gameBorder_left;
        int mouseY = pos.y / squareSize - gameBorder_top;

        sf::Event e;
        while (appWindow.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                appWindow.close();

            if (e.type == sf::Event::MouseButtonPressed && gameOver != 1)
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    if(first_click) // generate the map after the first click every time //
                    {
                        first_click=false;

                        board[mouseY * width + mouseX] = -2; // mark the position as a safe point //

                        // first things first (place the mines) //
                        placeMines(mouseY * width + mouseX);

                        // place the numbers //
                        for (int i = 0; i < squareNumber; i++)
                            countSurrounding(i % width, i / width);
                    }
                    if (boardRevealed[mouseY * width + mouseX] == 0 || boardRevealed[mouseY * width + mouseX] == 3)
                    {
                        if (board[mouseY * width + mouseX] == 9)
                        {
                            music.stop();
                            sound_gameOver.play();
                        }
                        else sound_mleft.play();
                        if(reveal(mouseX, mouseY))
                            board[mouseY * width + mouseX] = -1;
                    }
                }
                else if (mouseY>=0 && sf::Mouse::isButtonPressed(sf::Mouse::Right))
                {
                    if (boardRevealed[mouseY * width + mouseX] == 2)
                    {
                        sound_flag.play();
                        minesLeft++;
                        boardRevealed[mouseY * width + mouseX] = 3;
                    }
                    else if (boardRevealed[mouseY * width + mouseX] == 3)
                    {
                        sound_unflag.play();
                        boardRevealed[mouseY * width + mouseX] = 0;
                    }
                    else if (boardRevealed[mouseY * width + mouseX] == 0)
                    {
                        sound_flag.play();
                        minesLeft--;
                        boardRevealed[mouseY * width + mouseX] = 2;
                    }
                }
            }
        }

        // Draw the background blocks //
        appWindow.draw(background_sprite);

        // Face sprite control //
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if(mouseX==(width-1)/2 && mouseY==-3)
            {
                f.setTextureRect(sf::IntRect(squareSize*1,0,squareSize,squareSize));
                reset();
                sound_gameOver.stop();
                sound_gameWin.stop();
                music.play();
            }
            else
                f.setTextureRect(sf::IntRect(squareSize*3,0,squareSize,squareSize));
        }
        else if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            if(mouseX==(width-1)/2 && mouseY==-3)
                f.setTextureRect(sf::IntRect(squareSize*6,0,squareSize,squareSize));
        }
        else
            f.setTextureRect(sf::IntRect(squareSize*0,0,squareSize,squareSize));

        // Draw the face //
        appWindow.draw(f);

        for (int i = 0; i < squareNumber; i++)
        {
            if (boardRevealed[i] == 1 && board[i]==-1) // reveal the death position
                s.setTextureRect(sf::IntRect(10 * squareSize, 0, squareSize, squareSize));
            else if (boardRevealed[i] == 1) // reveal the i square
                s.setTextureRect(sf::IntRect(board[i] * squareSize, 0, squareSize, squareSize));
            else if (boardRevealed[i] == 0) // place the hide square
                s.setTextureRect(sf::IntRect(11 * squareSize, 0, squareSize, squareSize));
            else if (boardRevealed[i] == 2) // place the flag square
                s.setTextureRect(sf::IntRect(12 * squareSize, 0, squareSize, squareSize));
            else if (boardRevealed[i] == 3) // place the not for sure flag
                s.setTextureRect(sf::IntRect(13 * squareSize, 0, squareSize, squareSize));

            // Game End //
            // if the player won the game, places the flag on every bomb that wasn't marked
            if (board[i] == 9 && unrevealed == mineNumber && won && boardRevealed[i] != 2)
                s.setTextureRect(sf::IntRect(12 * squareSize, 0, squareSize, squareSize));
            // if the player lose the game, places the flag on every bomb that was marked
            else if (board[i] == 9 && gameOver && boardRevealed[i] == 2)
                s.setTextureRect(sf::IntRect(12 * squareSize, 0, squareSize, squareSize));
            // if the player lose the game, replaces the flag on every bomb that was marked wrong
            else if (board[i] != 9 && gameOver && boardRevealed[i] == 2)
                s.setTextureRect(sf::IntRect(13 * squareSize, 0, squareSize, squareSize));
            // in another case shows all the bombs
            else if (board[i] == 9 && gameOver)
                s.setTextureRect(sf::IntRect(9 * squareSize, 0, squareSize, squareSize));

            s.setPosition((i % width + gameBorder_left) * squareSize, (i / width + gameBorder_top) * squareSize);
            appWindow.draw(s);
        }

        // if won //
        if (unrevealed == mineNumber && !gameOver)
        {
            f.setTextureRect(sf::IntRect(squareSize*4,0,squareSize,squareSize));
            appWindow.draw(f);
            if (won == false)
            {
                won = true;
                music.stop();
                sound_gameWin.play();
            }
        }
        // else if lose //
        else if (gameOver == 1)
        {
            f.setTextureRect(sf::IntRect(squareSize*5,0,squareSize,squareSize));
            appWindow.draw(f);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if(mouseX==(width-1)/2 && mouseY==-3)
                {
                    f.setTextureRect(sf::IntRect(squareSize*1,0,squareSize,squareSize));
                    first_click=true;
                    reset();
                    gameOver=0;
                    music.play();
                }
            }
        }

        //appWindow.draw(scoreText);
        appWindow.display();
    }
    return 0;
}

int nearMines(int x, int y)
{
    if (x >= 0 && x < width && y >= 0 && y <= height)
    {
        if (board[y * width + x] == 9) return 1;
        else return 0;
    }
    else return 0;
}

void countSurrounding(int x, int y)
{
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

void placeMines(int first_click_poz)
{
    int count = 0;
     while (count < mineNumber) {
       int randPosition = rand() % squareNumber;
       if (board[randPosition] != 9 && board[randPosition]!=-2) {
         board[randPosition] = 9;
         ++count;
       }
     }
     if (board[first_click_poz]==-2)
         board[first_click_poz]=0;
}

bool revealed(int x, int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return true;
    if (boardRevealed[y * width + x] == 1 || boardRevealed[y * width + x] == 2) return true;
    return false;
}

void display(int x, int y, int firstX, int firstY)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    int i = y * width + x;
    if (boardRevealed[i] == 1 || boardRevealed[i] == 2 || board[i] == 9) return;
    boardRevealed[i] = 1;
    --unrevealed;
    if (board[i] == 0)
    {
        if (!revealed(x + 1, y))
        {
            display(x + 1, y, x, y);
        }
        if (!revealed(x - 1, y))
        {
            display(x - 1, y, x, y);
        }
        if (!revealed(x, y + 1))
        {
            display(x, y + 1, x, y);
        }
        if (!revealed(x, y - 1))
        {
            display(x, y - 1, x, y);
        }
        if (!revealed(x + 1, y + 1))
        {
            display(x + 1, y + 1, x, y);
        }
        if (!revealed(x + 1, y - 1))
        {
            display(x + 1, y - 1, x, y);
        }
        if (!revealed(x - 1, y + 1))
        {
            display(x - 1, y + 1, x, y);
        }
        if (!revealed(x - 1, y - 1))
        {
            display(x - 1, y - 1, x, y);
        }
    }
}

bool reveal(int x, int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    int i = y * width + x;
    if (boardRevealed[i] == 0 || boardRevealed[i] == 3)
    {
        boardRevealed[i] = 1;
        --unrevealed;
        if (board[i] == 0)
        {
            display(x + 1, y, x, y);
            display(x - 1, y, x, y);
            display(x, y + 1, x, y);
            display(x, y - 1, x, y);
            display(x + 1, y - 1, x, y);
            display(x + 1, y + 1, x, y);
            display(x - 1, y - 1, x, y);
            display(x - 1, y + 1, x, y);
        }
        if (board[i] == 9)
        {
            gameOver = true;
            return true;
        }
    }
    return false;
}

void reset()
{
    for(int i=0; i<squareNumber; i++)
    {
        board[i]=0;
        boardRevealed[i]=0;
    }
    gameOver = false;
    first_click = true;
    won = false;
    unrevealed = squareNumber;
}

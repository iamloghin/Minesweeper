#include "header.h"

int main()
{
    srand(time(0)); // the random function using the time in seconds since the Unix epoch as a new seed
    sf::Clock clock; // used to know the duration of the game
    sf::Time timer; // used to know the duration of the game

    // Create the window of the application //
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow appWindow(sf::VideoMode((width+gameBorder_right+gameBorder_left)*squareSize,(height+gameBorder_top+gameBorder_bottom)*squareSize, desktop.bitsPerPixel), "Minesweeper!", sf::Style::Resize);

    // Sounds & Music
    sf::SoundBuffer buffer_mleft, buffer_flag, buffer_unflag, buffer_gameOver, buffer_gameWin;
    sf::Sound sound_mleft, sound_flag, sound_unflag, sound_gameOver, sound_gameWin;
    sf::Music music, mainmenu;

    if (!buffer_mleft.loadFromFile("sounds/click.wav")			    ||
            !buffer_flag.loadFromFile("sounds/flag.wav")			||
            !buffer_unflag.loadFromFile("sounds/unflag.wav")		||
            !buffer_gameOver.loadFromFile("sounds/gameOver.wav")	||
            !buffer_gameWin.loadFromFile("sounds/gameWin.wav")		||
            !music.openFromFile("sounds/music.ogg")                 ||
            !mainmenu.openFromFile("sounds/mainmenu.ogg"))
    {
        std::cout << "can't find something in sounds :(" << std::endl;
    }

    // Change some parameters
    music.setPitch(1); // increase the pitch
    music.setVolume(35); // reduce the volume
    music.setLoop(true); // make it loop

    mainmenu.setPitch(1); // increase the pitch
    mainmenu.setVolume(30); // reduce the volume
    mainmenu.setLoop(true); // make it loop

    sound_mleft.setBuffer(buffer_mleft);
    sound_flag.setBuffer(buffer_flag);
    sound_unflag.setBuffer(buffer_unflag);
    sound_gameOver.setBuffer(buffer_gameOver);
    sound_gameWin.setBuffer(buffer_gameWin);

    // Load Icon
    sf::Image Icon;
    Icon.loadFromFile("images/icon.png");

    // Load the textures used in the game
    sf::Texture image_sprite, Background_image, faces, logo, menubuttons, optionbuttons;
    if( !image_sprite.loadFromFile("images/tiles.png")          ||
        !menubuttons.loadFromFile("images/menubuttons.png")     ||
        !faces.loadFromFile("images/faces.png")                 ||
        !logo.loadFromFile("images/logo.png")                   ||
        !optionbuttons.loadFromFile("images/optionbuttons.png") ||
        !Background_image.loadFromFile("images/tiles.png", sf::IntRect(15 * squareSize, 0, squareSize, squareSize)))
    {
        std::cout << "can't find all images :(" << std::endl;
    }

    sf::Sprite s(image_sprite), background_sprite(Background_image), f(faces), front_logo(logo), mainmenubutton(menubuttons), optionbuttonsmenu(optionbuttons);
    Background_image.setRepeated(true);
    background_sprite.setTextureRect(sf::IntRect(0,0,desktop.width,desktop.height));
    front_logo.setPosition(appWindow.getDefaultView().getSize().x/2-2.5*squareSize, 0);

    // Load the text font //
    sf::Font font;
    if (!font.loadFromFile("fonts/visitor1.ttf"))
        std::cout << "can't find the font (visitor1.ttf) :(" << std::endl;

    // Initialize the score text //
    sf::Text scoreText, gameTimer, menu[4];
    scoreText.setFont(font);
    scoreText.setColor(sf::Color::Red);
    scoreText.setCharacterSize(20);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setScale(2.f, 2.f);

    gameTimer.setFont(font);
    gameTimer.setColor(sf::Color::Red);
    gameTimer.setCharacterSize(20);
    gameTimer.setStyle(sf::Text::Bold);
    gameTimer.setScale(2.f, 2.f);

    // Main menu //

    menu[1].setFont(font);
    menu[1].setString("Play");
    menu[1].setPosition(appWindow.getDefaultView().getSize().x/2-squareSize-3, 4*squareSize-6);

    menu[2].setFont(font);
    menu[2].setString("Options");
    menu[2].setPosition(appWindow.getDefaultView().getSize().x/2-2*squareSize+8, 6*squareSize-6);

    menu[3].setFont(font);
    menu[3].setString("Exit");
    menu[3].setPosition(appWindow.getDefaultView().getSize().x/2-squareSize+3, 8*squareSize-6);

    // let's play the music //
    mainmenu.play();

    // Main loop //
    while (appWindow.isOpen())
    {        sf::Vector2i pos = sf::Mouse::getPosition(appWindow);
        int mouseX = pos.x / squareSize - gameBorder_left;
        int mouseY = pos.y / squareSize - gameBorder_top;

        switch(menustage)
        {
        case 0:
            sf::Event main_menu;
            while(appWindow.pollEvent(main_menu))
            {
                if(display_size)
                {
                    width=5;
                    height=5;
                    display_size=0;
                    appWindow.create(sf::VideoMode((width+gameBorder_right+gameBorder_left)*squareSize,(height+gameBorder_top+gameBorder_bottom)*squareSize, desktop.bitsPerPixel), "Minesweeper!");
                    appWindow.setIcon(32,32,Icon.getPixelsPtr());
                }
                appWindow.draw(background_sprite);

                // Draw the background //
                for(int i=3*width; i<8*width; i++)
                {
                    s.setTextureRect(sf::IntRect(16 * squareSize, 0, squareSize, squareSize));
                    s.setPosition((i % width + gameBorder_left) * squareSize, (i / width + 1) * squareSize);
                    appWindow.draw(s);
                    if(i==4*width-1)
                        i=5*width-1;
                    if(i==6*width-1)
                        i=7*width-1;
                }

                if(mouseY==0)
                    menu[1].setColor(sf::Color::Red);
                else if(mouseY==2)
                    menu[2].setColor(sf::Color::Red);
                else if(mouseY==4)
                    menu[3].setColor(sf::Color::Red);
                else
                {
                    menu[1].setColor(sf::Color::White);
                    menu[2].setColor(sf::Color::White);
                    menu[3].setColor(sf::Color::White);
                }

                appWindow.draw(front_logo);
                appWindow.draw(menu[1]);
                appWindow.draw(menu[2]);
                appWindow.draw(menu[3]);
                appWindow.display();

                if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouseY==0)
                {
                    menustage=3;
                    display_size=true;
                    if(sfx_sound)
                        sound_mleft.play();
                }
                else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouseY==2)
                {
                    menustage=2;
                    display_size=true;
                    if(sfx_sound)
                        sound_mleft.play();
                }
                else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouseY==4)
                    appWindow.close();
            }
            break;

        case 1:
            sf::Event play;
            while (appWindow.pollEvent(play))
            {
                if(display_size)
                {
                    minesLeft = mineNumber;
                    squareNumber = width * height;
                    unrevealed = squareNumber;
                    appWindow.setIcon(32,32,Icon.getPixelsPtr());
                    appWindow.create(sf::VideoMode((width+gameBorder_right+gameBorder_left)*squareSize,(height+gameBorder_top+gameBorder_bottom)*squareSize, desktop.bitsPerPixel), "Minesweeper!");
                    display_size=0;
                }
                if ((play.type == sf::Event::MouseButtonPressed || play.type == sf::Event::KeyPressed) && gameOver != 1 && unrevealed != mineNumber)
                {
                    if((mouseX >= 0) && (mouseX < width) && (mouseY >= 0) && (mouseY < height))
                    {
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        {
                            if(first_click) // generate the map after the first click every time //
                            {
                                first_click=false;

                                clock.restart(); // reset game time to 0

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
                                    if(music_sound)
                                        sound_gameOver.play();
                                }
                                else if(sfx_sound)
                                    sound_mleft.play();
                                if(reveal(mouseX, mouseY))
                                    board[mouseY * width + mouseX] = -1;
                            }
                        }
                        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                        {
                            if (boardRevealed[mouseY * width + mouseX] == 2)
                            {
                                if(sfx_sound)
                                    sound_flag.play();
                                minesLeft++;
                                if(mark_flag)
                                    boardRevealed[mouseY * width + mouseX] = 3;
                                else
                                    boardRevealed[mouseY * width + mouseX] = 0;
                            }
                            else if (boardRevealed[mouseY * width + mouseX] == 3)
                            {
                                if(sfx_sound)
                                    sound_unflag.play();
                                boardRevealed[mouseY * width + mouseX] = 0;
                            }
                            else if (boardRevealed[mouseY * width + mouseX] == 0 && minesLeft>-9)
                            {
                                if(sfx_sound)
                                    sound_flag.play();
                                minesLeft--;
                                boardRevealed[mouseY * width + mouseX] = 2;
                            }
                        }
                        else if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                        {
                            if(boardRevealed[mouseY * width + mouseX] == 1)
                                if(show_surroundings(mouseX, mouseY))
                                {
                                    music.stop();
                                    if(music_sound)
                                        sound_gameOver.play();
                                }
                        }
                    }
                }

            // Draw the background blocks //
            appWindow.draw(background_sprite);

            // Draw the background for the score table //
            if(width>=9)
            {
                for(int i=width; i<width*2; i++)
                {
                    s.setTextureRect(sf::IntRect(16 * squareSize, 0, squareSize, squareSize));
                    s.setPosition((i % width + gameBorder_left) * squareSize, (i / width + 1) * squareSize);
                    appWindow.draw(s);
                    if(i==width+2)
                        i=width*2-4;
                }
            }
            else
            {
                for(int i=width; i<width*2; i++)
                {
                    s.setTextureRect(sf::IntRect(16 * squareSize, 0, squareSize, squareSize));
                    s.setPosition((i % width + gameBorder_left) * squareSize, (i / width + 1) * squareSize);
                    appWindow.draw(s);
                    if(i==width+1)
                        i=width*2-3;
                }
            }

            // Update flagnumber //
            std::ostringstream flagleft;
            flagleft << minesLeft;
            scoreText.setString(flagleft.str());

            if(minesLeft>=100)
                scoreText.setPosition((squareSize+16), (gameBorder_top-3)*squareSize+17.6);
            else if(minesLeft<100 && minesLeft>=10)
                scoreText.setPosition((squareSize+16), (gameBorder_top-3)*squareSize+17.6);
            else if(minesLeft>=0 && minesLeft<=9)
                scoreText.setPosition((squareSize+20), (gameBorder_top-3)*squareSize+17.6);
            else
                scoreText.setPosition((squareSize+16), (gameBorder_top-3)*squareSize+17.6);
//------------------------------------------------------------------------------------------------

            // Update timer //
//--------------------------------------//
            if(first_click==false && gameOver != 1 && unrevealed != mineNumber){
            timer = clock.getElapsedTime();
            if(timer.asSeconds()>gameTime)
                if((width<9 && gameTime<99) || (width>=9 && gameTime<999))
                    gameTime++;
            }

            std::ostringstream timespend;
            timespend << gameTime;
            gameTimer.setString(timespend.str());

            if(gameTime>=100)
                gameTimer.setPosition((width-gameBorder_right*1.5)*squareSize, (gameBorder_top-3)*squareSize+17.6);
            else if(gameTime<100 && gameTime>=10)
                gameTimer.setPosition((width-gameBorder_right/1.5)*squareSize, (gameBorder_top-3)*squareSize+17.6);
            else
                gameTimer.setPosition((width-gameBorder_right/1.5)*squareSize, (gameBorder_top-3)*squareSize+17.6);

            appWindow.draw(gameTimer);
//------------------------------------------------------------------------------------------------

            // Face sprite control //

            // Set the face texture sprite depending on width
            if(width%2==0)
                faceSize=squareSize*2;
            else
                faceSize=squareSize;

            f.setPosition(appWindow.getDefaultView().getSize().x / 2 - faceSize/2, squareSize);
            unsigned int on_face=appWindow.getDefaultView().getSize().x / 2 - faceSize/2;
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if(pos.x>=on_face && pos.x<=on_face+faceSize && mouseY==-3)
                {
                    if(width%2==0)
                        f.setTextureRect(sf::IntRect(faceSize*1+squareSize*7,0,faceSize,squareSize));
                    else
                        f.setTextureRect(sf::IntRect(faceSize*1,0,faceSize,squareSize));
                    reset();
                    sound_gameOver.stop();
                    sound_gameWin.stop();
                    if(music_sound)
                        music.play();
                }
                else if(mouseX >= 0 && mouseX < width && mouseY >= 0 && mouseY < height)
                {
                    if(width%2==0)
                        f.setTextureRect(sf::IntRect(faceSize*3+squareSize*7,0,faceSize,squareSize));
                    else
                        f.setTextureRect(sf::IntRect(faceSize*3,0,faceSize,squareSize));
                }
            }
            else if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                if(pos.x>=on_face && pos.x<=on_face+faceSize && mouseY==-3)
                {
                    if(width%2==0)
                        f.setTextureRect(sf::IntRect(faceSize*6+squareSize*7,0,faceSize,squareSize));
                    else
                        f.setTextureRect(sf::IntRect(faceSize*6,0,faceSize,squareSize));
                }
            }
            else
            {
                if(width%2==0)
                    f.setTextureRect(sf::IntRect(faceSize*0+squareSize*7,0,faceSize,squareSize));
                else
                    f.setTextureRect(sf::IntRect(faceSize*0,0,faceSize,squareSize));
            }

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
                    s.setTextureRect(sf::IntRect(14 * squareSize, 0, squareSize, squareSize));
                // in another case shows all the bombs
                else if (board[i] == 9 && gameOver)
                    s.setTextureRect(sf::IntRect(9 * squareSize, 0, squareSize, squareSize));

                s.setPosition((i % width + gameBorder_left) * squareSize, (i / width + gameBorder_top) * squareSize);
                appWindow.draw(s);
            }

            // if won //
            if (unrevealed == mineNumber && !gameOver)
            {
                if(width%2==0)
                    f.setTextureRect(sf::IntRect(faceSize*4+squareSize*7,0,faceSize,squareSize));
                else
                    f.setTextureRect(sf::IntRect(faceSize*4,0,faceSize,squareSize));
                appWindow.draw(f);
                if (won == false)
                {
                    won = true;
                    music.stop();
                    minesLeft=0;
                    if(music_sound)
                        sound_gameWin.play();
                }
            }
            // else if lose //
            else if (gameOver == 1)
            {
                if(width%2==0)
                    f.setTextureRect(sf::IntRect(faceSize*5+squareSize*7,0,faceSize,squareSize));
                else
                    f.setTextureRect(sf::IntRect(faceSize*5,0,faceSize,squareSize));
                appWindow.draw(f);
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    if(mouseX==(width-1)/2 && mouseY==-3)
                    {
                        f.setTextureRect(sf::IntRect(squareSize*1,0,squareSize,squareSize));
                        first_click=true;
                        gameOver=0;
                        reset();
                        if(music_sound)
                            music.play();
                    }
                }
            }

            // Main Menu Game Buttons //
            mainmenubutton.setTextureRect(sf::IntRect(0 * squareSize, 0, squareSize, squareSize));
            mainmenubutton.setPosition(0, 0);
            appWindow.draw(mainmenubutton);
            mainmenubutton.setTextureRect(sf::IntRect(2 * squareSize, 0, squareSize, squareSize));
            mainmenubutton.setPosition(appWindow.getDefaultView().getSize().x-squareSize, 0);
            appWindow.draw(mainmenubutton);
            if(mouseY==-gameBorder_top && mouseX==-gameBorder_left)
            {
                mainmenubutton.setTextureRect(sf::IntRect(1 * squareSize, 0, squareSize, squareSize));
                mainmenubutton.setPosition(0, 0);
                appWindow.draw(mainmenubutton);
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    menustage=0;
                    display_size=1;
                    music.stop();
                    sound_gameWin.stop();
                    sound_gameOver.stop();
                    if(music_sound)
                        mainmenu.play();
                    if(sfx_sound)
                        sound_mleft.play();
                    reset();
                }
            }
            else if(mouseY==-gameBorder_top && mouseX==width)
            {
                mainmenubutton.setTextureRect(sf::IntRect(3 * squareSize, 0, squareSize, squareSize));
                mainmenubutton.setPosition(appWindow.getDefaultView().getSize().x-squareSize, 0);
                appWindow.draw(mainmenubutton);
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    appWindow.close();
            }

            appWindow.draw(f);
            appWindow.draw(scoreText);
            appWindow.display();
            }
            break;

        case 2:
            sf::Event options_menu;
            while(appWindow.pollEvent(options_menu))
            {
                if(display_size)
                {
                    display_size=0;
                    appWindow.setIcon(32,32,Icon.getPixelsPtr());
                }

                // Draw background //
                appWindow.draw(background_sprite);

                // Draw Title: Options //
                optionbuttonsmenu.setTextureRect(sf::IntRect(0, 0, 161, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left+squareSize-1, squareSize);
                appWindow.draw(optionbuttonsmenu);

                // Draw Back Minesweeper Face Button //
                if(mark_flag)
                {
                    s.setTextureRect(sf::IntRect(13*squareSize, 0, squareSize, squareSize));
                    s.setPosition(appWindow.getDefaultView().getSize().x / 2 - squareSize/2, squareSize*3);
                    appWindow.draw(s);
                }
                else
                {
                    s.setTextureRect(sf::IntRect(12*squareSize, 0, squareSize, squareSize));
                    s.setPosition(appWindow.getDefaultView().getSize().x / 2 - squareSize/2, squareSize*3);
                    appWindow.draw(s);
                }

                // Draw Music Button //
                if(music_sound)
                {
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5, 0, 64, squareSize));
                    optionbuttonsmenu.setPosition(gameBorder_left+squareSize-2, squareSize*4);
                    appWindow.draw(optionbuttonsmenu);
                }
                else
                {
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64, 0, 64, squareSize));
                    optionbuttonsmenu.setPosition(gameBorder_left+squareSize-2, squareSize*4);
                    appWindow.draw(optionbuttonsmenu);
                }

                // Draw Sound Effect Button //
                if(sfx_sound)
                {
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*2, 0, 64, squareSize));
                    optionbuttonsmenu.setPosition(appWindow.getDefaultView().getSize().x / 2+squareSize/2, squareSize*4);
                    appWindow.draw(optionbuttonsmenu);
                }
                else
                {
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*3, 0, 64, squareSize));
                    optionbuttonsmenu.setPosition(appWindow.getDefaultView().getSize().x / 2+squareSize/2, squareSize*4);
                    appWindow.draw(optionbuttonsmenu);
                }

                // Draw High Score Button //
                optionbuttonsmenu.setTextureRect(sf::IntRect(160, 0, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left+squareSize-2, squareSize*6);
                appWindow.draw(optionbuttonsmenu);

                // Draw Help Button //
                optionbuttonsmenu.setTextureRect(sf::IntRect(160*3, 0, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left+squareSize, squareSize*8);
                appWindow.draw(optionbuttonsmenu);

                if(mouseY==-1 && mouseX==2)
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        if(mark_flag)
                            mark_flag=false;
                        else
                            mark_flag=true;
                        if(sfx_sound)
                            sound_mleft.play();
                    }

                if(mouseY==0 && mouseX>=0 && mouseX<=1)
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        if(sfx_sound)
                            sound_mleft.play();
                        if(music_sound)
                        {
                            mainmenu.stop();
                            music_sound=false;
                        }
                        else
                        {
                            music_sound=true;
                            mainmenu.play();
                        }
                    }

                if(mouseY==0 && mouseX>=3 && mouseX<=4)
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        if(sfx_sound)
                            sfx_sound=false;
                        else
                            sfx_sound=true;
                        sound_mleft.play();
                    }

                if(mouseY==2 && mouseX>=0 && mouseX<=4)
                {
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*2, 0, 160, squareSize));
                    optionbuttonsmenu.setPosition(gameBorder_left+squareSize-2, squareSize*6);
                    appWindow.draw(optionbuttonsmenu);
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        if(sfx_sound)
                            sound_mleft.play();
                }

                if(mouseY==4 && mouseX>=0 && mouseX<=4)
                {
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*4, 0, 160, squareSize));
                    optionbuttonsmenu.setPosition(gameBorder_left+squareSize, squareSize*8);
                    appWindow.draw(optionbuttonsmenu);
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        if(sfx_sound)
                            sound_mleft.play();
                }

                    // Main Menu Game Buttons //
                mainmenubutton.setTextureRect(sf::IntRect(0 * squareSize, 0, squareSize, squareSize));
                mainmenubutton.setPosition(0, 0);
                appWindow.draw(mainmenubutton);
                mainmenubutton.setTextureRect(sf::IntRect(2 * squareSize, 0, squareSize, squareSize));
                mainmenubutton.setPosition(appWindow.getDefaultView().getSize().x-squareSize, 0);
                appWindow.draw(mainmenubutton);
                if(mouseY==-gameBorder_top && mouseX==-gameBorder_left)
                {
                    mainmenubutton.setTextureRect(sf::IntRect(1 * squareSize, 0, squareSize, squareSize));
                    mainmenubutton.setPosition(0, 0);
                    appWindow.draw(mainmenubutton);
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        menustage=0;
                }
                else if(mouseY==-gameBorder_top && mouseX==width)
                {
                    mainmenubutton.setTextureRect(sf::IntRect(3 * squareSize, 0, squareSize, squareSize));
                    mainmenubutton.setPosition(appWindow.getDefaultView().getSize().x-squareSize, 0);
                    appWindow.draw(mainmenubutton);
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        appWindow.close();
                }


                appWindow.display();
            }
            break;

        case 3:
            sf::Event playmode;
            while (appWindow.pollEvent(playmode))
            {
                if(display_size)
                {
                    width=5;
                    height=6;
                    appWindow.setIcon(32,32,Icon.getPixelsPtr());
                    appWindow.create(sf::VideoMode((width+gameBorder_right+gameBorder_left)*squareSize,(height+gameBorder_top+gameBorder_bottom)*squareSize, desktop.bitsPerPixel), "Minesweeper!");
                    display_size=0;
                }
                // Draw background //
                appWindow.draw(background_sprite);

                // Draw Title: Options //
                optionbuttonsmenu.setTextureRect(sf::IntRect(0, squareSize, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left+squareSize-1, squareSize);
                appWindow.draw(optionbuttonsmenu);

                // Draw Game: Begginer //
                optionbuttonsmenu.setTextureRect(sf::IntRect(160, squareSize, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left+squareSize-1, squareSize*3);
                appWindow.draw(optionbuttonsmenu);

                // Draw Game: Intermediate //
                optionbuttonsmenu.setTextureRect(sf::IntRect(160*3, squareSize, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left+squareSize-1, squareSize*5);
                appWindow.draw(optionbuttonsmenu);

                // Draw Game: Expert //
                optionbuttonsmenu.setTextureRect(sf::IntRect(160*5, squareSize, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left+squareSize-1, squareSize*7);
                appWindow.draw(optionbuttonsmenu);

                // Draw Game: Custom //
                optionbuttonsmenu.setTextureRect(sf::IntRect(160*7, squareSize, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left+squareSize-1, squareSize*9);
                appWindow.draw(optionbuttonsmenu);

                if(mouseY==-1 && mouseX>=0 && mouseX<=4)
                {
                optionbuttonsmenu.setTextureRect(sf::IntRect(160*2, squareSize, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left+squareSize-1, squareSize*3);
                appWindow.draw(optionbuttonsmenu);
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        width=9;
                        height=9;
                        mineNumber=10;
                        display_size=true;
                        menustage=1;
                        if(music_sound)
                        {
                            mainmenu.stop();
                            music.play();
                        }
                        if(sfx_sound)
                            sound_mleft.play();
                    }
                }

                if(mouseY==1 && mouseX>=0 && mouseX<=4)
                {
                optionbuttonsmenu.setTextureRect(sf::IntRect(160*4, squareSize, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left+squareSize-1, squareSize*5);
                appWindow.draw(optionbuttonsmenu);
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        width=16;
                        height=16;
                        mineNumber=40;
                        display_size=true;
                        menustage=1;
                        if(music_sound)
                        {
                            mainmenu.stop();
                            music.play();
                        }
                        if(sfx_sound)
                            sound_mleft.play();
                    }
                }

                if(mouseY==3 && mouseX>=0 && mouseX<=4)
                {
                optionbuttonsmenu.setTextureRect(sf::IntRect(160*6, squareSize, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left+squareSize-1, squareSize*7);
                appWindow.draw(optionbuttonsmenu);
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        width=30;
                        height=16;
                        mineNumber=99;
                        display_size=true;
                        menustage=1;
                        if(music_sound)
                        {
                            mainmenu.stop();
                            music.play();
                        }
                        if(sfx_sound)
                            sound_mleft.play();
                    }
                }

                if(mouseY==5 && mouseX>=0 && mouseX<=4)
                {
                optionbuttonsmenu.setTextureRect(sf::IntRect(160*8, squareSize, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left+squareSize-1, squareSize*9);
                appWindow.draw(optionbuttonsmenu);
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        if(sfx_sound)
                            sound_mleft.play();
                    }
                }

                // Main Menu Game Buttons //
                mainmenubutton.setTextureRect(sf::IntRect(0 * squareSize, 0, squareSize, squareSize));
                mainmenubutton.setPosition(0, 0);
                appWindow.draw(mainmenubutton);
                mainmenubutton.setTextureRect(sf::IntRect(2 * squareSize, 0, squareSize, squareSize));
                mainmenubutton.setPosition(appWindow.getDefaultView().getSize().x-squareSize, 0);
                appWindow.draw(mainmenubutton);
                if(mouseY==-gameBorder_top && mouseX==-gameBorder_left)
                {
                    mainmenubutton.setTextureRect(sf::IntRect(1 * squareSize, 0, squareSize, squareSize));
                    mainmenubutton.setPosition(0, 0);
                    appWindow.draw(mainmenubutton);
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        menustage=0;
                }
                else if(mouseY==-gameBorder_top && mouseX==width)
                {
                    mainmenubutton.setTextureRect(sf::IntRect(3 * squareSize, 0, squareSize, squareSize));
                    mainmenubutton.setPosition(appWindow.getDefaultView().getSize().x-squareSize, 0);
                    appWindow.draw(mainmenubutton);
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        appWindow.close();
                }
                appWindow.display();
            }
            break;
        }
    }
    return 0;
}

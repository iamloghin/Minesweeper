#include "header.h"

int main()
{
    srand(time(0)); // the random function using the time in seconds since the Unix epoch as a new seed
    sf::Clock clock, clock2; // used to know the duration of the game
    sf::Time timer, timer2; // used to know the duration of the game

    // Create the window of the application //
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow appWindow(sf::VideoMode((width+gameBorder_right+gameBorder_left)*squareSize,(height+gameBorder_top+gameBorder_bottom)*squareSize, desktop.bitsPerPixel), "Minesweeper!", sf::Style::Resize);

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile("images/icon.png"))
    {
        std::cout << "can't find the game icon :(" << std::endl;
    }

    // Sounds & Music
    sf::SoundBuffer buffer_mleft, buffer_flag, buffer_unflag, buffer_gameOver, buffer_gameWin, buffer_wrong;
    sf::Sound sound_mleft, sound_flag, sound_unflag, sound_gameOver, sound_gameWin, sound_wrong;
    sf::Music music, mainmenu;

    if (!buffer_mleft.loadFromFile("sounds/click.wav")			    ||
            !buffer_flag.loadFromFile("sounds/flag.wav")			||
            !buffer_unflag.loadFromFile("sounds/unflag.wav")		||
            !buffer_gameOver.loadFromFile("sounds/gameOver.wav")	||
            !buffer_gameWin.loadFromFile("sounds/gameWin.wav")		||
            !buffer_wrong.loadFromFile("sounds/wrong.wav")		    ||
            !music.openFromFile("sounds/music.ogg")                 ||
            !mainmenu.openFromFile("sounds/mainmenu.ogg"))
    {
        std::cout << "can't find something in sounds :(" << std::endl;
    }

    // Change some parameters //
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
    sound_wrong.setBuffer(buffer_wrong);

    // Load the textures used in the game //
    sf::Texture image_sprite, Background_image, faces, logo, optionbuttons, tutorial;
    if( !image_sprite.loadFromFile("images/tiles.png")          ||
        !faces.loadFromFile("images/faces.png")                 ||
        !tutorial.loadFromFile("images/tutorial.png")           ||
        !logo.loadFromFile("images/logo.png")                   ||
        !optionbuttons.loadFromFile("images/optionbuttons.png") ||
        !Background_image.loadFromFile("images/tiles.png", sf::IntRect(16 * squareSize, 0, squareSize, squareSize)))
    {
        std::cout << "can't find all images :(" << std::endl;
    }

    // Insert texture in sprite //
    sf::Sprite s(image_sprite), background_sprite(Background_image), f(faces), front_logo(logo), optionbuttonsmenu(optionbuttons), tutorial_image(tutorial);
    Background_image.setRepeated(true);
    background_sprite.setTextureRect(sf::IntRect(0,0,desktop.width,desktop.height));
    front_logo.setOrigin(sf::Vector2f(80, 48));
    front_logo.setPosition(appWindow.getDefaultView().getSize().x/2, squareSize*2-10);

    // Load the text font //
    sf::Font font;
    if (!font.loadFromFile("fonts/visitor1.ttf"))
        std::cout << "can't find the font (visitor1.ttf) :(" << std::endl;

    // Initialize the game text //
    sf::String playerstring[4];
    sf::Text scoreText, gameTimer, game_version, player_movesText, menu[4], playerinput[4];

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

    player_movesText.setFont(font);
    player_movesText.setColor(sf::Color::Green);
    player_movesText.setCharacterSize(20);
    player_movesText.setStyle(sf::Text::Bold);
    player_movesText.setScale(2.f, 2.f);

    game_version.setFont(font);
    game_version.setString("1.1.0");
    game_version.setPosition(appWindow.getDefaultView().getSize().x/2.5, 8*squareSize-6);

    // Custom Game Player Input //

    playerinput[1].setFont(font);
    playerinput[1].setColor(sf::Color::Green);
    playerinput[1].setCharacterSize(20);
    playerinput[1].setStyle(sf::Text::Bold);
    playerinput[1].setScale(2.f, 2.f);

    playerinput[2].setFont(font);
    playerinput[2].setColor(sf::Color::Green);
    playerinput[2].setCharacterSize(20);
    playerinput[2].setStyle(sf::Text::Bold);
    playerinput[2].setScale(2.f, 2.f);

    playerinput[3].setFont(font);
    playerinput[3].setColor(sf::Color::Green);
    playerinput[3].setCharacterSize(20);
    playerinput[3].setStyle(sf::Text::Bold);
    playerinput[3].setScale(2.f, 2.f);

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
    {
        sf::Vector2i pos = sf::Mouse::getPosition(appWindow);
        int mouseX = pos.x / squareSize - gameBorder_left;
        int mouseY = pos.y / squareSize - gameBorder_top;

        switch(menustage)
        {
        case 0:
            sf::Event main_menu;
            if(appWindow.isOpen())
            {
                if(display_size)
                {
                    width=5;
                    height=5;
                    display_size=0;
                    appWindow.create(sf::VideoMode((width+gameBorder_right+gameBorder_left)*squareSize,(height+gameBorder_top+gameBorder_bottom)*squareSize, desktop.bitsPerPixel), "Minesweeper!");
                    appWindow.setIcon(icon.getSize().x, icon.getSize().y,icon.getPixelsPtr());
                }

                // Draw the background //
                appWindow.draw(background_sprite);

                // Draw the Logo image //
                timer2 = clock2.restart();
                logo_animation+= timer2.asSeconds();
                front_logo.setScale(lerp(0.98, 1.05, abs(cos(logo_animation))), lerp(0.98, 1.05, abs(cos(logo_animation))));
                appWindow.draw(front_logo);

                // Draw the text background //
                for(int i=3*width; i<8*width; i++)
                {
                    s.setTextureRect(sf::IntRect(17 * squareSize, 0, squareSize, squareSize));
                    s.setPosition((i % width + gameBorder_left) * squareSize, (i / width + 1) * squareSize);
                    appWindow.draw(s);
                    if(i==4*width-1)
                        i=5*width-1;
                    if(i==6*width-1)
                        i=7*width-1;
                }

                // Draw the text + Hover //
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

                // Draw the text menu //
                for(int i=1; i<4; i++)
                    appWindow.draw(menu[i]);
            }
            while(appWindow.pollEvent(main_menu))
            {
                switch(main_menu.type)
                {
                case sf::Event::MouseButtonPressed:
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        mouse_button=1;
                case sf::Event::MouseButtonReleased:
                    if(mouse_button)
                    {
                        mouse_button=0;
                        if(mouseY==0)
                        {
                            menustage=3;
                            display_size=true;
                            if(sfx_sound)
                                sound_mleft.play();
                        }
                        else if(mouseY==2)
                        {
                            menustage=2;
                            display_size=true;
                            if(sfx_sound)
                                sound_mleft.play();
                        }
                        else if(mouseY==4)
                            appWindow.close();
                    }
                    break;
                default:
                    break;
                }
            }
            appWindow.display();
            break;

        case 1:
            sf::Event play;
            if(appWindow.isOpen())
            {
                if(display_size)
                {
                    minesLeft = mineNumber;
                    squareNumber = width * height;
                    unrevealed = squareNumber;
                    appWindow.create(sf::VideoMode((width+gameBorder_right+gameBorder_left)*squareSize,(height+gameBorder_top+gameBorder_bottom)*squareSize, desktop.bitsPerPixel), "Minesweeper!");
                    appWindow.setIcon(icon.getSize().x, icon.getSize().y,icon.getPixelsPtr());
                    display_size=0;
                }

                // Draw the background blocks //
                appWindow.draw(background_sprite);

                // Main Menu Game Button //
                if(mouseY==-gameBorder_top && mouseX==-gameBorder_left)
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32, 0, squareSize, squareSize));
                else
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4, 0, squareSize, squareSize));
                    optionbuttonsmenu.setPosition(0, 0);
                    appWindow.draw(optionbuttonsmenu);

                // Exit Game Button //
                if(mouseY==-gameBorder_top && mouseX==width)
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32*3, 0, squareSize, squareSize));
                else
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32*2, 0, squareSize, squareSize));
                optionbuttonsmenu.setPosition(appWindow.getDefaultView().getSize().x-squareSize, 0);
                appWindow.draw(optionbuttonsmenu);

                // Draw the background for the score table //
                if(width>=9)
                {
                    for(int i=width; i<width*2; i++)
                    {
                        s.setTextureRect(sf::IntRect(17 * squareSize, 0, squareSize, squareSize));
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
                        s.setTextureRect(sf::IntRect(17 * squareSize, 0, squareSize, squareSize));
                        s.setPosition((i % width + gameBorder_left) * squareSize, (i / width + 1) * squareSize);
                        appWindow.draw(s);
                        if(i==width+1)
                            i=width*2-3;
                    }
                }

                // Update game flag number //
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

                // Game timer update //
                if(first_click==false && gameOver != 1 && unrevealed != mineNumber){
                timer = clock.getElapsedTime();
                if(timer.asSeconds()>gameTime)
                    if((width<9 && gameTime<99) || (width>=9 && gameTime<999))
                        gameTime++;
                }

                std::ostringstream timespend, moves;
                moves << player_moves;
                timespend << gameTime;
                player_movesText.setString(moves.str());
                gameTimer.setString(timespend.str());

                if(gameTime>=100 || player_moves>=100)
                {
                    gameTimer.setPosition((width-gameBorder_right*1.5)*squareSize, squareSize+17.6);
                    player_movesText.setPosition((width-gameBorder_right*1.5)*squareSize, squareSize+17.6);
                }
                else if(gameTime<100 && gameTime>=10 || player_moves<100 && player_moves>=10)
                {
                    gameTimer.setPosition((width-gameBorder_right/1.5)*squareSize, squareSize+17.6);
                    player_movesText.setPosition((width-gameBorder_right/1.5)*squareSize, squareSize+17.6);
                }
                else
                {
                    gameTimer.setPosition((width-gameBorder_right/1.5)*squareSize, squareSize+17.6);
                    player_movesText.setPosition((width-gameBorder_right/1.5)*squareSize, squareSize+17.6);
                }

                // Face sprite control //
                // Set the face texture sprite depending on width //
                if(width%2==0)
                    faceSize=squareSize*2;
                else
                    faceSize=squareSize;

                // Set the face position //
                f.setPosition(appWindow.getDefaultView().getSize().x / 2 - faceSize/2, squareSize);

                unsigned int on_face=appWindow.getDefaultView().getSize().x / 2 - faceSize/2;
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)   ||
                   sf::Mouse::isButtonPressed(sf::Mouse::Middle) ||
                   sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                {
                    if(pos.x>=on_face && pos.x<=on_face+faceSize && mouseY==-3)
                    {
                        if(width%2==0)
                            f.setTextureRect(sf::IntRect(faceSize*1+squareSize*7,0,faceSize,squareSize));
                        else
                            f.setTextureRect(sf::IntRect(faceSize*1,0,faceSize,squareSize));
                        if(gameOver==1 || unrevealed == mineNumber)
                        {
                            sound_gameOver.stop();
                            sound_gameWin.stop();
                            if(music_sound)
                                music.play();
                        }
                        reset();
                    }
                    else if(mouseX >= 0 && mouseX < width && mouseY >= 0 && mouseY < height)
                    {
                        if(width%2==0)
                            f.setTextureRect(sf::IntRect(faceSize*3+squareSize*7,0,faceSize,squareSize));
                        else
                            f.setTextureRect(sf::IntRect(faceSize*3,0,faceSize,squareSize));
                    }
                }
                else if(sf::Mouse::isButtonPressed(sf::Mouse::Right) ||
                        sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
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

                // Draw the game board //
                for (int i = 0; i < squareNumber; i++)
                {
                    if (boardRevealed[i] == 1 && board[i]==-1) // reveal the death position //
                        s.setTextureRect(sf::IntRect(10 * squareSize, 0, squareSize, squareSize));
                    else if (boardRevealed[i] == 1) // reveal the i square //
                        s.setTextureRect(sf::IntRect(board[i] * squareSize, 0, squareSize, squareSize));
                    else if (boardRevealed[i] == 0) // place the hide square //
                        s.setTextureRect(sf::IntRect(11 * squareSize, 0, squareSize, squareSize));
                    else if (boardRevealed[i] == 2) // place the flag square //
                        s.setTextureRect(sf::IntRect(12 * squareSize, 0, squareSize, squareSize));
                    else if (boardRevealed[i] == 3) // place the not for sure flag //
                        s.setTextureRect(sf::IntRect(13 * squareSize, 0, squareSize, squareSize));

                    // Game End //
                    // if the player won the game, places the flag on every bomb that wasn't marked //
                    if (board[i] == 9 && unrevealed == mineNumber && won && boardRevealed[i] != 2)
                        s.setTextureRect(sf::IntRect(12 * squareSize, 0, squareSize, squareSize));
                    // if the player lose the game, places the flag on every bomb that was marked //
                    else if (board[i] == 9 && gameOver && boardRevealed[i] == 2)
                        s.setTextureRect(sf::IntRect(12 * squareSize, 0, squareSize, squareSize));
                    // if the player lose the game, replaces the flag on every bomb that was marked wrong //
                    else if (board[i] != 9 && gameOver && boardRevealed[i] == 2)
                        s.setTextureRect(sf::IntRect(15 * squareSize, 0, squareSize, squareSize));
                    // in another case shows all the bombs //
                    else if (board[i] == 9 && gameOver)
                        s.setTextureRect(sf::IntRect(9 * squareSize, 0, squareSize, squareSize));


                    s.setPosition((i % width + gameBorder_left) * squareSize, (i / width + gameBorder_top) * squareSize);
                    appWindow.draw(s);
                }

                //focus on the block where the player hold the left mouse press
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && (boardRevealed[mouseX+width*mouseY] == 0 || boardRevealed[mouseX+width*mouseY] == 3))
                {
                    if((mouseX >= 0) && (mouseX < width) && (mouseY >= 0) && (mouseY < height))
                    {
                        if(boardRevealed[mouseX+width*mouseY] == 3)
                            s.setTextureRect(sf::IntRect(14 * squareSize, 0, squareSize, squareSize));
                        else
                            s.setTextureRect(sf::IntRect(0 * squareSize, 0, squareSize, squareSize));
                        s.setPosition((gameBorder_left+mouseX)*squareSize, (gameBorder_top+mouseY)*squareSize);
                        if(gameOver != 1 && unrevealed != mineNumber)
                            appWindow.draw(s);
                    }
                }

                //focus on the block where the player hold the  middle mouse press or LShift
                else if(sf::Mouse::isButtonPressed(sf::Mouse::Middle) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                {
                    for(int i=-1; i<=1; i++)
                        for(int j=-1; j<=1; j++)
                            if(boardRevealed[(mouseX+i)+width*(mouseY+j)] == 0 || boardRevealed[(mouseX+i)+width*(mouseY+j)] == 3)
                            {
                                if(mouseX+i>=0 && mouseX+i<width && mouseY+j>=0 && mouseY+j<height)
                                {
                                if(boardRevealed[(mouseX+i)+width*(mouseY+j)] == 3)
                                        s.setTextureRect(sf::IntRect(14 * squareSize, 0, squareSize, squareSize));
                                    else
                                        s.setTextureRect(sf::IntRect(0 * squareSize, 0, squareSize, squareSize));
                                    s.setPosition((gameBorder_left+mouseX+i)*squareSize, (gameBorder_top+mouseY+j)*squareSize);
                                    if(gameOver != 1 && unrevealed != mineNumber)
                                        appWindow.draw(s);
                                }
                            }
                }

                // If game win //
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

                // If game lose //
                if (gameOver == 1)
                {
                    if(width%2==0)
                        f.setTextureRect(sf::IntRect(faceSize*5+squareSize*7,0,faceSize,squareSize));
                    else
                        f.setTextureRect(sf::IntRect(faceSize*5,0,faceSize,squareSize));
                    appWindow.draw(f);
                }

                appWindow.draw(f);
                appWindow.draw(scoreText);
                if(show_moves)
                    appWindow.draw(player_movesText);
                else appWindow.draw(gameTimer);;
            }
            while(appWindow.pollEvent(play))
            {
                if (play.type == sf::Event::Closed)
                    appWindow.close();
                if((gameOver != 1) && (unrevealed != mineNumber) && (mouseX >= 0) && (mouseX < width) && (mouseY >= 0) && (mouseY < height))
                {
                    switch(play.type)
                    {
                    case sf::Event::MouseButtonPressed:
                        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        {
                            player_moves++;
                            mouse_button=1;
                        }
                        else if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
                        {
                            player_moves++;
                            mouse_button=2;
                        }
                        else  if(sf::Mouse::isButtonPressed(sf::Mouse::Middle))
                        {
                            player_moves++;
                            mouse_button=3;
                        }
                        break;
                    case sf::Event::KeyPressed:
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                        {
                            player_moves++;
                            mouse_button=2;
                        }
                        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                        {
                            player_moves++;
                            mouse_button=3;
                        }
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
                        {
                            if(gameOver==1 || unrevealed == mineNumber)
                            {
                                sound_gameOver.stop();
                                sound_gameWin.stop();
                                if(music_sound)
                                    music.play();
                            }
                            reset();
                        }
                        break;
                    case sf::Event::KeyReleased:
                        if(mouse_button==2)
                        {
                            if (boardRevealed[mouseY * width + mouseX] == 2)
                            {
                                minesLeft++;
                                if(sfx_sound)
                                    sound_flag.play();
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
                                minesLeft--;
                                boardRevealed[mouseY * width + mouseX] = 2;
                                if(sfx_sound)
                                    sound_flag.play();
                            }
                            mouse_button=0;
                        }
                        else if(mouse_button==3)
                        {
                            if(boardRevealed[mouseY * width + mouseX] == 1)
                                if(show_surroundings(mouseX, mouseY))
                                {
                                    music.stop();
                                    if(music_sound)
                                        sound_gameOver.play();
                                }
                            mouse_button=0;
                        }
                        break;
                    case sf::Event::MouseButtonReleased:
                        if(mouse_button==1)
                        {
                            if(first_click)
                            {
                                // generate the map after the first click every time //
                                first_click=false;
                                clock.restart();

                                // mark the position as a safe point //
                                board[mouseY * width + mouseX] = -2;

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
                            mouse_button=0;
                        }
                        else if(mouse_button==2)
                        {
                            if (boardRevealed[mouseY * width + mouseX] == 2)
                            {
                                minesLeft++;
                                if(sfx_sound)
                                    sound_flag.play();
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
                                minesLeft--;
                                boardRevealed[mouseY * width + mouseX] = 2;
                                if(sfx_sound)
                                    sound_flag.play();
                            }
                            mouse_button=0;
                        }
                        else if(mouse_button==3)
                        {
                            if(boardRevealed[mouseY * width + mouseX] == 1)
                                if(show_surroundings(mouseX, mouseY))
                                {
                                    music.stop();
                                    if(music_sound)
                                        sound_gameOver.play();
                                }
                            mouse_button=0;
                        }
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    switch(play.type)
                    {
                    case sf::Event::MouseButtonPressed:
                        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                            mouse_button=1;
                    case sf::Event::MouseButtonReleased:
                        if(mouse_button==1)
                        {
                            if(mouseY==-(gameBorder_top-2) && mouseX<=width-gameBorder_right)
                            {
                                if(width<9)
                                {
                                    if(mouseX>=width-gameBorder_right-1)
                                    {
                                        mouse_button=0;
                                        if(show_moves)
                                            show_moves=false;
                                        else
                                            show_moves=true;
                                    }
                                }
                                else if(mouseX>=width-gameBorder_right-2)
                                    {
                                        mouse_button=0;
                                        if(show_moves)
                                            show_moves=false;
                                        else
                                            show_moves=true;
                                    }
                            }
                            if(mouseY==-gameBorder_top && mouseX==-gameBorder_left)
                            {
                                mouse_button=0;
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
                            if(mouseY==-gameBorder_top && mouseX==width)
                            {
                                mouse_button=0;
                                appWindow.close();
                            }
                        }
                        break;
                    case sf::Event::KeyPressed:
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
                        {
                            if(gameOver==1 || unrevealed == mineNumber)
                            {
                                sound_gameOver.stop();
                                sound_gameWin.stop();
                                if(music_sound)
                                    music.play();
                            }
                            reset();
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
            appWindow.display();
            break;

        case 2:
            sf::Event options_menu;
            if(appWindow.isOpen())
            {
                if(display_size)
                {
                    display_size=0;
                    appWindow.setIcon(icon.getSize().x, icon.getSize().y,icon.getPixelsPtr());
                }

                // Draw background //
                appWindow.draw(background_sprite);

                // Main Menu Game Button //
                if(mouseY==-gameBorder_top && mouseX==-gameBorder_left)
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32, 0, squareSize, squareSize));
                else
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4, 0, squareSize, squareSize));
                    optionbuttonsmenu.setPosition(0, 0);
                    appWindow.draw(optionbuttonsmenu);

                // Exit Game Button //
                if(mouseY==-gameBorder_top && mouseX==width)
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32*3, 0, squareSize, squareSize));
                else
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32*2, 0, squareSize, squareSize));
                optionbuttonsmenu.setPosition(appWindow.getDefaultView().getSize().x-squareSize, 0);
                appWindow.draw(optionbuttonsmenu);

                // Draw Title: Options //
                optionbuttonsmenu.setTextureRect(sf::IntRect(0, 0, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize);
                appWindow.draw(optionbuttonsmenu);

                // Draw Back Minesweeper Flag Button //
                if(mark_flag)
                    s.setTextureRect(sf::IntRect(13*squareSize, 0, squareSize, squareSize));
                else
                    s.setTextureRect(sf::IntRect(12*squareSize, 0, squareSize, squareSize));
                s.setPosition(appWindow.getDefaultView().getSize().x / 2 - squareSize/2, squareSize*3);
                appWindow.draw(s);

                // Draw Music Button //
                if(music_sound)
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5, 0, 64, squareSize));
                else
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64, 0, 64, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize*4);
                appWindow.draw(optionbuttonsmenu);

                // Draw Sound Effect Button //
                if(sfx_sound)
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*2, 0, 64, squareSize));
                else
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*3, 0, 64, squareSize));
                optionbuttonsmenu.setPosition(appWindow.getDefaultView().getSize().x / 2+squareSize/2, squareSize*4);
                appWindow.draw(optionbuttonsmenu);

                // Draw Help Button //
                optionbuttonsmenu.setTextureRect(sf::IntRect(160*3, 0, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize*6);
                appWindow.draw(optionbuttonsmenu);

                // Draw Help Button Hover//
                if(mouseY==2 && mouseX>=0 && mouseX<=4)
                {
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*4, 0, 160, squareSize));
                    optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize*6);
                    appWindow.draw(optionbuttonsmenu);
                }

                // Draw About Button //
                optionbuttonsmenu.setTextureRect(sf::IntRect(160, 0, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize*8);
                appWindow.draw(optionbuttonsmenu);

                // Draw About Button Hover//
                if(mouseY==4 && mouseX>=0 && mouseX<=4)
                {
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*2, 0, 160, squareSize));
                    optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize*8);
                    appWindow.draw(optionbuttonsmenu);
                }
            }
            while(appWindow.pollEvent(options_menu))
            {
                switch(options_menu.type)
                {
                case sf::Event::MouseButtonPressed:
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        mouse_button=1;
                case sf::Event::MouseButtonReleased:
                    if(mouse_button)
                    {
                        mouse_button=0;
                        if(mouseY==-1 && mouseX==2)
                        {
                            if(mark_flag)
                                mark_flag=false;
                            else
                                mark_flag=true;
                            if(sfx_sound)
                                sound_mleft.play();
                        }
                        if(mouseY==0 && mouseX>=0 && mouseX<=1)
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
                        {
                            if(sfx_sound)
                                sfx_sound=false;
                            else
                                sfx_sound=true;
                            sound_mleft.play();
                        }
                        if(mouseY==2 && mouseX>=0 && mouseX<=4)
                        {
                            width=6;
                            height=6;
                            display_size=true;
                            menustage=5;
                            if(sfx_sound)
                                sound_mleft.play();
                        }
                        if(mouseY==4 && mouseX>=0 && mouseX<=4)
                        {
                            menustage=6;
                            display_size=true;
                            if(sfx_sound)
                                sound_mleft.play();
                        }
                        if(mouseY==-gameBorder_top && mouseX==-gameBorder_left)
                            menustage=0;
                        if(mouseY==-gameBorder_top && mouseX==width)
                            appWindow.close();
                    }
                    break;
                }
            }
            appWindow.display();
            break;

        case 3:
            sf::Event playmode;
            if(appWindow.isOpen())
            {
                if(display_size)
                {
                    width=5;
                    height=6;
                    appWindow.create(sf::VideoMode((width+gameBorder_right+gameBorder_left)*squareSize,(height+gameBorder_top+gameBorder_bottom)*squareSize, desktop.bitsPerPixel), "Minesweeper!");
                    appWindow.setIcon(icon.getSize().x, icon.getSize().y,icon.getPixelsPtr());
                    display_size=0;
                }

                // Draw background //
                appWindow.draw(background_sprite);

                // Main Menu Game Button //
                if(mouseY==-gameBorder_top && mouseX==-gameBorder_left)
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32, 0, squareSize, squareSize));
                else
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4, 0, squareSize, squareSize));
                    optionbuttonsmenu.setPosition(0, 0);
                    appWindow.draw(optionbuttonsmenu);

                // Exit Game Button //
                if(mouseY==-gameBorder_top && mouseX==width)
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32*3, 0, squareSize, squareSize));
                else
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32*2, 0, squareSize, squareSize));
                optionbuttonsmenu.setPosition(appWindow.getDefaultView().getSize().x-squareSize, 0);
                appWindow.draw(optionbuttonsmenu);

                // Draw the choices //
                for(unsigned int i=0; i<=7; i++)
                {
                    if(i==0)
                    {
                        // Draw Title: Options //
                        optionbuttonsmenu.setTextureRect(sf::IntRect(0, squareSize, 160, squareSize));
                        optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize);
                        appWindow.draw(optionbuttonsmenu);
                    }
                    else
                    {
                        // Draw Game Mode: Begginer, Intermediate, Expert, Custom //
                        optionbuttonsmenu.setTextureRect(sf::IntRect(160*i, squareSize, 160, squareSize));
                        optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize*(i+2));
                        appWindow.draw(optionbuttonsmenu);
                        i++;
                    }
                }

                // Draw Game Mode: Begginer, Intermediate, Expert, Custom  Hover //
                if((mouseY==-1 || mouseY==1 || mouseY==3 || mouseY==5) && mouseX>=0 && mouseX<=4)
                {
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*(4+mouseY-1), squareSize, 160, squareSize));
                    optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize*(4+mouseY));
                    appWindow.draw(optionbuttonsmenu);
                }
            }
            while(appWindow.pollEvent(playmode))
            {
                switch(playmode.type)
                {
                case sf::Event::MouseButtonPressed:
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        mouse_button=1;
                    break;
                case sf::Event::MouseButtonReleased:
                    if(mouse_button)
                    {
                        mouse_button=0;
                        if(mouseY==-1 && mouseX>=0 && mouseX<=4)
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
                        if(mouseY==1 && mouseX>=0 && mouseX<=4)
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
                        if(mouseY==3 && mouseX>=0 && mouseX<=4)
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
                        if(mouseY==5 && mouseX>=0 && mouseX<=4)
                        {
                            menustage=4;
                            display_size=true;
                            if(sfx_sound)
                                sound_mleft.play();
                        }
                        if(mouseY==-gameBorder_top && mouseX==-gameBorder_left)
                        {
                            menustage=0;
                            display_size=1;
                        }
                        if(mouseY==-gameBorder_top && mouseX==width)
                            appWindow.close();
                    }
                    break;
                }
            }
            appWindow.display();
            break;

        case 4:
            sf::Event custom;
            if(appWindow.isOpen())
            {
               if(display_size)
                {
                    customw=0;
                    customh=0;
                    customm=0;
                    for(int i=1; i<4; i++)
                    {
                        playerstring[i].clear();
                        playerinput[i].setString(playerstring[i]);
                    }
                    appWindow.setIcon(icon.getSize().x, icon.getSize().y,icon.getPixelsPtr());
                    display_size=0;
                }

                // Draw background //
                appWindow.draw(background_sprite);

                // Main Menu Game Button //
                if(mouseY==-gameBorder_top && mouseX==-gameBorder_left)
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32, 0, squareSize, squareSize));
                else
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4, 0, squareSize, squareSize));
                    optionbuttonsmenu.setPosition(0, 0);
                    appWindow.draw(optionbuttonsmenu);

                // Exit Game Button //
                if(mouseY==-gameBorder_top && mouseX==width)
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32*3, 0, squareSize, squareSize));
                else
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32*2, 0, squareSize, squareSize));
                optionbuttonsmenu.setPosition(appWindow.getDefaultView().getSize().x-squareSize, 0);
                appWindow.draw(optionbuttonsmenu);

                // Draw the choices //
                for(unsigned int i=0; i<=7; i++)
                    if(i==0)
                    {
                        // Draw Title: Options //
                        optionbuttonsmenu.setTextureRect(sf::IntRect(0, squareSize*2, 160, squareSize));
                        optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize);
                        appWindow.draw(optionbuttonsmenu);
                    }
                    else
                    {
                        // Draw Game Mode: Begginer, Intermediate, Expert, Custom //
                        optionbuttonsmenu.setTextureRect(sf::IntRect(160*i, squareSize*2, 160, squareSize));
                        optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize*(i+2));
                        appWindow.draw(optionbuttonsmenu);
                        i++;
                    }

                // Draw Game Mode: Begginer, Intermediate, Expert, Custom  Hover //
                if((mouseY==0 || mouseY==2 || mouseY==4 || mouseY==5) && mouseX>=0 && mouseX<=4)
                {

                    if(mouseY==5)
                    {
                        optionbuttonsmenu.setTextureRect(sf::IntRect(160*(mouseY+3), squareSize*2, 160, squareSize));
                        optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize*(mouseY+4));
                    }
                    else
                    {
                        optionbuttonsmenu.setTextureRect(sf::IntRect(160*(mouseY+2), squareSize*2, 160, squareSize));
                        optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize*(mouseY+3));
                    }
                    appWindow.draw(optionbuttonsmenu);
                }

                // Draw the text input background //
                for(int i=3*width; i<8*width; i++)
                {
                    s.setTextureRect(sf::IntRect(17 * squareSize, 0, squareSize, squareSize));
                    s.setPosition((i % width + gameBorder_left) * squareSize, (i / width + 1) * squareSize);
                    appWindow.draw(s);
                    if(i==4*width-1)
                        i=5*width-1;
                    if(i==6*width-1)
                        i=7*width-1;
                }
                for(int i=1; i<4; i++)
                    appWindow.draw(playerinput[i]);
            }
            while (appWindow.pollEvent(custom))
            {
                switch(custom.type)
                {
                case sf::Event::TextEntered:
                    if(mouseY==0 && mouseX>=0 && mouseX<=4)
                    {
                        if(custom.text.unicode == '\b')
                            playerstring[1].erase(playerstring[1].getSize()-1,1);
                        else if(custom.text.unicode <128)
                            playerstring[1] +=custom.text.unicode;
                        playerinput[1].setString(playerstring[1]);
                        playerinput[1].setPosition(appWindow.getDefaultView().getSize().x/2-playerstring[1].getSize()*10, gameBorder_top+squareSize*4-18);
                        std::stringstream convert(playerstring[1]);
                        convert >> customw;
                        if(customw>18) sound_wrong.play();
                    }
                    if(mouseY==2 && mouseX>=0 && mouseX<=4)
                    {
                        if(custom.text.unicode == '\b')
                            playerstring[2].erase(playerstring[2].getSize()-1,1);
                        else if(custom.text.unicode <128)
                            playerstring[2] +=custom.text.unicode;
                        playerinput[2].setString(playerstring[2]);
                        playerinput[2].setPosition(appWindow.getDefaultView().getSize().x/2-playerstring[2].getSize()*10, gameBorder_top+squareSize*6-18);
                        std::stringstream convert(playerstring[2]);
                        convert >> customh;
                        if(customh>18) sound_wrong.play();
                    }
                    if(mouseY==4 && mouseX>=0 && mouseX<=4)
                    {
                        if(custom.text.unicode == '\b')
                            playerstring[3].erase(playerstring[3].getSize()-1,1);
                        else if(custom.text.unicode <128)
                            playerstring[3] +=custom.text.unicode;
                        playerinput[3].setString(playerstring[3]);
                        playerinput[3].setPosition(appWindow.getDefaultView().getSize().x/2-playerstring[3].getSize()*10, gameBorder_top+squareSize*8-18);
                        std::stringstream convert(playerstring[3]);
                        convert >> customm;
                        if(customm>320) sound_wrong.play();
                    }
                    if(customw>18 || customw<5) playerinput[1].setColor(sf::Color::Red);
                    else playerinput[1].setColor(sf::Color::Green);
                    if(customh>18 || customh<5) playerinput[2].setColor(sf::Color::Red);
                    else playerinput[2].setColor(sf::Color::Green);
                    if(customm>320 || customm<5) playerinput[3].setColor(sf::Color::Red);
                    else playerinput[3].setColor(sf::Color::Green);
                    break;
                case sf::Event::MouseButtonPressed:
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        mouse_button=1;
                    break;
                case sf::Event::MouseButtonReleased:
                    if(mouse_button)
                    {
                        mouse_button=0;
                        if(mouseY==5 && mouseX>=0 && mouseX<=4)
                        {
                            if(customw!=0 && customh!=0 && customm!=0)
                            {
                                if(customw<5 || customw>18)
                                    width=5;
                                else
                                    width=customw;
                                if(customh<5 || customh>18)
                                    height=5;
                                else
                                    height=customh;
                                if(customm<5)
                                    mineNumber=5;
                                else if(customm>=width*height || customm>320)
                                    mineNumber=width*height-1;
                                else
                                    mineNumber=customm;
                                menustage=1;
                                display_size=true;
                                if(music_sound)
                                {
                                    mainmenu.stop();
                                    music.play();
                                }
                                if(sfx_sound)
                                    sound_mleft.play();
                            }
                            else
                                sound_wrong.play();
                        }
                        if(mouseY==-gameBorder_top && mouseX==-gameBorder_left)
                        {
                            display_size=1;
                            menustage=0;
                        }
                        if(mouseY==-gameBorder_top && mouseX==width)
                            appWindow.close();
                    }
                    break;
                }
            }
            appWindow.display();
            break;

        case 5:
            sf::Event tutorial_menu;
            if(appWindow.isOpen())
            {
                if(display_size)
                {
                    display_size=0;
                    tutorialw=0;
                    tutorialh=0;
                    appWindow.create(sf::VideoMode((width+gameBorder_right+gameBorder_left)*squareSize,(height+gameBorder_top+gameBorder_bottom)*squareSize, desktop.bitsPerPixel), "Minesweeper!");
                    appWindow.setIcon(icon.getSize().x, icon.getSize().y,icon.getPixelsPtr());
                }

                // Draw background //
                appWindow.draw(background_sprite);

                // Main Menu Game Button //
                if(mouseY==-gameBorder_top && mouseX==-gameBorder_left)
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32, 0, squareSize, squareSize));
                else
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4, 0, squareSize, squareSize));
                    optionbuttonsmenu.setPosition(0, 0);
                    appWindow.draw(optionbuttonsmenu);

                // Exit Game Button //
                if(mouseY==-gameBorder_top && mouseX==width)
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32*3, 0, squareSize, squareSize));
                else
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32*2, 0, squareSize, squareSize));
                optionbuttonsmenu.setPosition(appWindow.getDefaultView().getSize().x-squareSize, 0);
                appWindow.draw(optionbuttonsmenu);

                // Draw Title: Tutorial //
                optionbuttonsmenu.setTextureRect(sf::IntRect(0, squareSize*4, 192, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize);
                appWindow.draw(optionbuttonsmenu);

                // Draw Click Button //
                optionbuttonsmenu.setTextureRect(sf::IntRect(192, squareSize*4, 192, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize*9);
                appWindow.draw(optionbuttonsmenu);

                // Draw Click Button Hover//
                if(mouseY==5 && mouseX>=0 && mouseX<=4)
                {
                    optionbuttonsmenu.setTextureRect(sf::IntRect(192*2, squareSize*4, 192, squareSize));
                    optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize*9);
                    appWindow.draw(optionbuttonsmenu);
                }

                // Draw step i from tutorial //
                tutorial_image.setTextureRect(sf::IntRect(tutorialw*192, tutorialh*192, 192, 192));
                tutorial_image.setPosition(gameBorder_left*squareSize, (gameBorder_top-1)*squareSize);
                appWindow.draw(tutorial_image);
            }
            while(appWindow.pollEvent(tutorial_menu))
            {
                switch(tutorial_menu.type)
                {
                case sf::Event::MouseButtonPressed:
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        mouse_button=1;
                case sf::Event::MouseButtonReleased:
                    if(mouse_button)
                    {
                        mouse_button=0;
                        if(mouseY==5 && mouseX>=0 && mouseX<=4)
                        {
                            if(tutorialw==10 && tutorialh==2)
                            {
                                menustage=3;
                                display_size=true;
                            }
                            if(tutorialw==10)
                            {
                                tutorialw=0;
                                tutorialh++;
                            }
                            else tutorialw++;
                            if(sfx_sound)
                                sound_mleft.play();
                        }
                        if(mouseY==-gameBorder_top && mouseX==-gameBorder_left)
                        {
                            menustage=0;
                            display_size=true;
                        }
                        if(mouseY==-gameBorder_top && mouseX==width)
                            appWindow.close();
                    }
                    break;
                }
            }
            appWindow.display();
            break;

        case 6:
            sf::Event about_menu;
            if(appWindow.isOpen())
            {
                if(display_size)
                {
                    display_size=0;
                    appWindow.setIcon(icon.getSize().x, icon.getSize().y,icon.getPixelsPtr());
                }

                // Draw background //
                appWindow.draw(background_sprite);

                // Main Menu Game Button //
                if(mouseY==-gameBorder_top && mouseX==-gameBorder_left)
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32, 0, squareSize, squareSize));
                else
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4, 0, squareSize, squareSize));
                    optionbuttonsmenu.setPosition(0, 0);
                    appWindow.draw(optionbuttonsmenu);

                // Exit Game Button //
                if(mouseY==-gameBorder_top && mouseX==width)
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32*3, 0, squareSize, squareSize));
                else
                    optionbuttonsmenu.setTextureRect(sf::IntRect(160*5+64*4+32*2, 0, squareSize, squareSize));
                optionbuttonsmenu.setPosition(appWindow.getDefaultView().getSize().x-squareSize, 0);
                appWindow.draw(optionbuttonsmenu);

                // Draw Title: About //
                optionbuttonsmenu.setTextureRect(sf::IntRect(0, squareSize*3, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize);
                appWindow.draw(optionbuttonsmenu);

                // Draw Octocat Button //
                if(mouseY==-1 && mouseX==2)
                    optionbuttonsmenu.setTextureRect(sf::IntRect(31*squareSize, squareSize*3, squareSize, squareSize));
                else
                    optionbuttonsmenu.setTextureRect(sf::IntRect(30*squareSize, squareSize*3, squareSize, squareSize));
                optionbuttonsmenu.setPosition(appWindow.getDefaultView().getSize().x / 2 - squareSize/2, squareSize*3);
                appWindow.draw(optionbuttonsmenu);

                // Draw Made by Button //
                optionbuttonsmenu.setTextureRect(sf::IntRect(5*squareSize, squareSize*3, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize*4);
                appWindow.draw(optionbuttonsmenu);

                // Draw Made by Hover//
                if(mouseY==1 && mouseX>=0 && mouseX<=4)
                {
                    optionbuttonsmenu.setTextureRect(sf::IntRect(10*squareSize, squareSize*3, 160, squareSize));
                    optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize*4);
                    appWindow.draw(optionbuttonsmenu);
                }

                // Draw Version Button //
                optionbuttonsmenu.setTextureRect(sf::IntRect(15*squareSize, squareSize*3, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize*5);
                appWindow.draw(optionbuttonsmenu);

                // Draw Version Button //
                optionbuttonsmenu.setTextureRect(sf::IntRect(20*squareSize, squareSize*3, 160, squareSize));
                optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize*7);
                appWindow.draw(optionbuttonsmenu);

                // Draw Version Hover//
                if(mouseY==4 && mouseX>=0 && mouseX<=4)
                {
                    optionbuttonsmenu.setTextureRect(sf::IntRect(25*squareSize, squareSize*3, 160, squareSize));
                    optionbuttonsmenu.setPosition(gameBorder_left*squareSize, squareSize*7);
                    appWindow.draw(optionbuttonsmenu);
                }

                // Draw the text background //
                for(int i=7*width; i<8*width; i++)
                {
                    s.setTextureRect(sf::IntRect(17 * squareSize, 0, squareSize, squareSize));
                    s.setPosition((i % width + gameBorder_left) * squareSize, (i / width + 1) * squareSize);
                    appWindow.draw(s);
                }

                appWindow.draw(game_version);
            }
            while(appWindow.pollEvent(about_menu))
            {
                switch(about_menu.type)
                {
                case sf::Event::MouseButtonPressed:
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        mouse_button=1;
                case sf::Event::MouseButtonReleased:
                    if(mouse_button)
                    {
                        mouse_button=0;
                        if(mouseY==-1 && mouseX==2)
                        {
                            ShellExecute(NULL, "open", "https://github.com/logalex96", NULL, NULL, SW_SHOWNORMAL);
                            if(sfx_sound)
                                sound_mleft.play();
                        }
                        if(mouseY==-gameBorder_top && mouseX==-gameBorder_left)
                            menustage=0;
                        if(mouseY==-gameBorder_top && mouseX==width)
                            appWindow.close();
                    }
                    break;
                }
            }
            appWindow.display();
            break;

        default:
            appWindow.close();
            break;
        }
    }
    return 0;
}

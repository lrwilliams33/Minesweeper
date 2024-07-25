#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "board.h"
#include "tile.h"
#include <vector>
#include "leaderboard.h"
#include "texture.h"
using namespace std;

void createText(sf::Text& text, const sf::Font& font, const string& textToWrite, int charSize, float xPos, float yPos, bool isBold, bool isUnderlined){
    text.setFont(font);
    text.setString(textToWrite);
    text.setCharacterSize(charSize);
    float textWidth = text.getLocalBounds().width;
    float textHeight = text.getLocalBounds().height;
    text.setOrigin(textWidth / 2, textHeight / 2);
    text.setPosition(xPos, yPos);
    text.setFillColor(sf::Color::White);
    if (isBold && isUnderlined){text.setStyle(sf::Text::Bold | sf::Text::Underlined);}
    if (isBold){text.setStyle(sf::Text::Bold);}
}

// returns vector of all tiles surrounding clicked tile
vector<Tile> findSurroundingTiles(const vector<vector<Tile>>& tilesVect, int row, int col){
    vector<Tile> surroundingVect;
    surroundingVect.push_back(tilesVect[row][col]);

    try{surroundingVect.push_back(tilesVect.at(row).at(col-1));} // middle left
    catch(...){}
    try{surroundingVect.push_back(tilesVect.at(row).at(col+1));} // middle right
    catch(...){}
    try{surroundingVect.push_back(tilesVect.at(row-1).at(col));} // middle top
    catch(...){}
    try{surroundingVect.push_back(tilesVect.at(row+1).at(col));} // middle bottom
    catch(...){}
    try{surroundingVect.push_back(tilesVect.at(row-1).at(col-1));} // top left corner
    catch(...){}
    try{surroundingVect.push_back(tilesVect.at(row-1).at(col+1));} // top right corner
    catch(...){}
    try{surroundingVect.push_back(tilesVect.at(row+1).at(col-1));} // bottom left corner
    catch(...){}
    try{surroundingVect.push_back(tilesVect.at(row+1).at(col+1));} // bottom right corner
    catch(...){}

    return surroundingVect;
}

void setTexture(vector<vector<Tile>>& tilesVect, TextureManager& Textures, int row, int col){
    if (tilesVect[row][col]._numNearbyMines == 1){tilesVect[row][col]._numSprite.setTexture(Textures.oneTexture);}
    if (tilesVect[row][col]._numNearbyMines == 2){tilesVect[row][col]._numSprite.setTexture(Textures.twoTexture);}
    if (tilesVect[row][col]._numNearbyMines == 3){tilesVect[row][col]._numSprite.setTexture(Textures.threeTexture);}
    if (tilesVect[row][col]._numNearbyMines == 4){tilesVect[row][col]._numSprite.setTexture(Textures.fourTexture);}
    if (tilesVect[row][col]._numNearbyMines == 5){tilesVect[row][col]._numSprite.setTexture(Textures.fiveTexture);}
    if (tilesVect[row][col]._numNearbyMines == 6){tilesVect[row][col]._numSprite.setTexture(Textures.sixTexture);}
    if (tilesVect[row][col]._numNearbyMines == 7){tilesVect[row][col]._numSprite.setTexture(Textures.sevenTexture);}
    if (tilesVect[row][col]._numNearbyMines == 8){tilesVect[row][col]._numSprite.setTexture(Textures.eightTexture);}

}

// optimize by adding class variable of nearby tiles and if it has the vector already, dont call findSurroundingTiles()
void revealSurroundingTiles(vector<vector<Tile>>& tilesVect, vector<Tile>& surroundingVect, int numRows, int numCols, TextureManager& Textures){
    if (!tilesVect[surroundingVect.at(0)._row][surroundingVect.at(0)._col]._isRevealed){
        tilesVect[surroundingVect.at(0)._row][surroundingVect.at(0)._col].revealEmptyTile(Textures.revealedTexture);
    }
    for (int i=1; i<surroundingVect.size(); ++i){
        Tile currentTile = surroundingVect.at(i);
        if (currentTile._numNearbyMines == 0 && !currentTile._isRevealed){
            tilesVect[currentTile._row][currentTile._col].revealEmptyTile(Textures.revealedTexture);
            auto newSurroundingVect = findSurroundingTiles(tilesVect, currentTile._row, currentTile._col);
            revealSurroundingTiles(tilesVect, newSurroundingVect, numRows, numCols, Textures);
        }
        else if (currentTile._numNearbyMines < 9 && !currentTile._isRevealed){
            setTexture(tilesVect, Textures, currentTile._row, currentTile._col);
            tilesVect[currentTile._row][currentTile._col]._tileSprite.setTexture(Textures.revealedTexture);
            tilesVect[currentTile._row][currentTile._col]._isRevealed = true;
            tilesVect[currentTile._row][currentTile._col]._hasFlag = false;
        }
    }
}

bool gameWin(vector<vector<Tile>>& tilesVect, const int& numRows, const int& numCols){
    for (int i=0; i<numRows; ++i){
        for (int j=0; j<numCols; ++j){
            if (!tilesVect[i][j]._isRevealed && !tilesVect[i][j]._isMine){return false;}
            // if (tilesVect[i][j]._isMine && !tilesVect[i][j]._hasFlag){return false;}
        }}
    return true;
}

int main()
{
    std::fstream config;
    int numColumns = 0;
    int numRows = 0;
    int numMines = 0;
    config.open("files/board_config.cfg", ios_base::in);
    if (config.is_open()){config >> numColumns; config >> numRows; config >> numMines;}
    else{ cout << "could not open file";}
    // read config file

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")){cout << "could not load font" << endl;}
    // load the font

    int windowWidth = numColumns * 32;
    int windowHeight = (numRows * 32) + 100;
    string playerName;
    sf::RenderWindow titleWindow(sf::VideoMode(windowWidth, windowHeight), "Minesweeper", sf::Style::Close);
    // make titleWindow

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("WELCOME TO MINESWEEPER!");
    titleText.setCharacterSize(24);
    float textWidth = titleText.getLocalBounds().width;
    float textHeight = titleText.getLocalBounds().height;
    titleText.setOrigin(textWidth / 2, textHeight / 2);
    titleText.setPosition((float)windowWidth / 2.0f - textWidth / 2.0f, (float)windowHeight / 2.0f - 150);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    // welcome to minesweeper titleText

    sf::Text namePrompt;
    namePrompt.setFont(font);
    namePrompt.setString("Enter your name:");
    namePrompt.setCharacterSize(24);
    float namePromptWidth = namePrompt.getLocalBounds().width;
    float namePromptHeight = namePrompt.getLocalBounds().height;
    namePrompt.setOrigin(namePromptWidth / 2, namePromptHeight / 2);
    namePrompt.setPosition((float)windowWidth / 2.0f, (float)windowHeight / 2.0f-75);
    namePrompt.setFillColor(sf::Color::White);
    namePrompt.setStyle(sf::Text::Bold);
    // enter your name titleText

    string str = "|";
    sf::Text playerText;
    playerText.setFont(font);
    playerText.setString("|");
    playerText.setCharacterSize(24);
    float playerTextWidth = playerText.getLocalBounds().width;
    float playerTextHeight = playerText.getLocalBounds().height;
    titleText.setOrigin(playerTextWidth / 2, playerTextHeight / 2);
    playerText.setPosition((float)windowWidth / 2.0f, (float)windowHeight / 2.0f-45);
    playerText.setFillColor(sf::Color::White);
    playerText.setStyle(sf::Text::Bold);
    // input name titleText

    // create text for leaderboard window
    float leaderboardWindowWidth = 16 * numColumns;
    float leaderboardWindowHeight = (16 * numRows) + 50;
    float leadXpos = leaderboardWindowWidth / 2.0f;
    float leadYpos = (leaderboardWindowHeight / 2.0f) - 120;
    sf::Text leaderboardText;
    createText(leaderboardText, font, "LEADERBOARD", 20, leadXpos, leadYpos, true, true);
    sf::Text namesText;
    float namesXpos = leadXpos;
    float namesYpos = (leaderboardWindowHeight / 2.0f) + 20;

    bool pressedClose = false;
    while (titleWindow.isOpen())
    {
        sf::Event event{};
        while (titleWindow.pollEvent(event)){
            // close window
            if (event.type == sf::Event::Closed){titleWindow.close(); pressedClose = true;}

            // input titleText for name
            if (event.type == sf::Event::TextEntered){
                if (str.size() <= 11) {
                    if (event.text.unicode == '\b' && str.size() >= 2) { str.erase(str.size() - 2, 1); }
                    else if (isalpha(event.text.unicode) && str.size() <= 10) {
                        str.erase(str.size() - 1, 1);
                        str += event.text.unicode;
                        str += '|';
                    }
                }

                // add new letter to string and realign to center
                playerText.setString(str);
                playerText.setOrigin(playerText.getLocalBounds().width / 2.0f, playerText.getLocalBounds().height / 2.0f);
                playerText.setPosition((float)windowWidth / 2.0f, (float)windowHeight / 2.0f -  35);

                // if enter is pressed, alter name and save to playerName
                if(event.text.unicode == '\r' && str.size() > 1){
                    str[0] = toupper(str[0]);
                    str.erase(str.size() - 1, 1);
                    for (int i=1; i<str.size(); i++){
                        str[i] = tolower(str[i]);}
                    playerName = str;
                    titleWindow.close();
                }
            }
        }
        // draw all things needed in the window
        titleWindow.clear(sf::Color::Blue);
        titleWindow.draw(titleText);
        titleWindow.draw(namePrompt);
        titleWindow.draw(playerText);
        titleWindow.display();

    }
    //// End of title window...............................................................................

    if (!pressedClose) {
        // make game window
        sf::RenderWindow gameWindow(sf::VideoMode(windowWidth, windowHeight), "Minesweeper", sf::Style::Close);
        // sf::RenderWindow gameWindow(sf::VideoMode(windowWidth, windowHeight), "Minesweeper"); // game window that can be resized and moved


        // make textures
        TextureManager Textures;
        Textures.loadTextures();

        // make board
        Board board(numRows, numColumns, numMines);
        board.placeMines();
        board.addNumNearbyMines();
        board.printBoard();
        auto boardArr = board.getBoard();
        bool gameOver = false;
        bool gameIsWon = false;

        // get tile size
        int tileWidth = windowWidth / numColumns;
        int tileHeight = (windowHeight - 100) / numRows;

        // sets the opening game look
        sf::Texture hiddenTexture;
        hiddenTexture.loadFromFile("files/images/tile_hidden.png");
        sf::Texture revealedTexture;
        revealedTexture.loadFromFile("files/images/tile_revealed.png");
        hiddenTexture.setRepeated(true);
        revealedTexture.setRepeated(true);
        sf::Sprite hiddenTilesprite;
        hiddenTilesprite.setTexture(hiddenTexture);
        hiddenTilesprite.setTextureRect({0, 0, windowWidth, windowHeight - 100});
        auto hiddenTileBounds = hiddenTilesprite.getGlobalBounds();

        // create happy face button
        sf::Texture happyTexture;
        happyTexture.loadFromFile("files/images/face_happy.png");
        sf::Texture faceWinTexture;
        faceWinTexture.loadFromFile("files/images/face_win.png");
        sf::Texture faceLostTexture;
        faceLostTexture.loadFromFile("files/images/face_lose.png");
        sf::Sprite faceSprite;
        faceSprite.setTexture(happyTexture);
        faceSprite.setPosition(((numColumns / 2.0f) * 32) - 32, 32 * ((numRows) + 0.5f));
        auto faceBounds = faceSprite.getGlobalBounds();
        int numFlagsPressed = 0;
        int numFlags = numMines;
        bool negativeFlags = false;

        // create debug button
        sf::Texture debugTexture;
        debugTexture.loadFromFile("files/images/debug.png");
        sf::Sprite debugSprite;
        debugSprite.setTexture(debugTexture);
        float xPos = ((numColumns) * 32) - 304;
        float yPos = 32 * ((numRows) + 0.5f);
        debugSprite.setPosition(xPos, yPos);
        auto debugBounds = debugSprite.getGlobalBounds();
        bool debugIsPressed = false;

        // create play/pause button
        sf::Texture pauseTexture;
        pauseTexture.loadFromFile("files/images/pause.png");
        sf::Texture playTexture;
        playTexture.loadFromFile("files/images/play.png");
        sf::Sprite pauseSprite;
        pauseSprite.setTexture(pauseTexture);
        xPos = ((numColumns) * 32) - 240;
        yPos = 32 * ((numRows) + 0.5f);
        pauseSprite.setPosition(xPos, yPos);
        auto pauseBounds = pauseSprite.getGlobalBounds();
        bool pauseIsPressed = false;

        // create leaderboard button
        sf::Texture leaderboardTexture;
        leaderboardTexture.loadFromFile("files/images/leaderboard.png");
        sf::Sprite leaderboardSprite;
        leaderboardSprite.setTexture(leaderboardTexture);
        xPos = ((numColumns) * 32) - 176;
        yPos = 32 * ((numRows) + 0.5f);
        leaderboardSprite.setPosition(xPos, yPos);
        auto leaderboardBounds = leaderboardSprite.getGlobalBounds();
        bool leaderboardIsPressed = false;
        bool gameWonPass = false;

        gameWindow.clear();
        gameWindow.draw(hiddenTilesprite);
        gameWindow.draw(faceSprite);
        gameWindow.draw(debugSprite);
        gameWindow.draw(pauseSprite);
        gameWindow.draw(leaderboardSprite);
        gameWindow.display();


        // create vector of tiles
        vector<vector<Tile>> tilesVect;
        for (int row = 0; row < numRows; row++) {
            vector<Tile> temp;
            for (int col = 0; col < numColumns; col++) {
                Tile newTile(col * tileWidth, row * tileHeight, row, col);
                newTile._numNearbyMines = boardArr[row][col];
                if (boardArr[row][col] == 9) { newTile._isMine = true; }
                temp.push_back(newTile);
            }
            tilesVect.push_back(temp);
        }

        // make clock
        sf::Clock clock;
        int evenTime = -1;
        int oddTime = -1;
        int counter = 0;
        int totalTimeSecs = -1;
        float tensMinPos = ((float) numColumns * 32) - 97;
        float onesMinPos = tensMinPos + 21;
        float tensSecPos = ((float) numColumns * 32) - 54;
        float onesSecPos = tensSecPos + 21;
        float digitYPos = (((float) numRows + 0.5f) * 32) + 16;
        int mins;
        int secs;
        int ind = 0;

        // create vector of digit sprites
        auto digitsVect = Textures.digitsVect();
        vector<sf::Sprite> digitSprites;
        for (int digit = 0; digit < 11; digit++) {
            sf::Sprite newSprite;
            newSprite.setTexture(digitsVect[digit]);
            digitSprites.push_back(newSprite);
        }

        // execute while game window is open
        while (gameWindow.isOpen()) {
            sf::Event event{};
            while (gameWindow.pollEvent(event)) {
                // close window
                if (event.type == sf::Event::Closed) { gameWindow.close(); }

                // if right mouse button pressed
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right &&
                    !gameOver && !gameIsWon && !pauseIsPressed) {
                    auto mouseCoord = sf::Mouse::getPosition(gameWindow);
                    if (hiddenTileBounds.contains((float) mouseCoord.x, (float) mouseCoord.y)) {
                        // determines which row and col have been pressed
                        int cellWidth = gameWindow.getSize().x / numColumns;
                        int cellHeight = (gameWindow.getSize().y - 100) / numRows;
                        int mouseCol = mouseCoord.x / cellWidth;
                        int mouseRow = mouseCoord.y / cellHeight;
                        Tile clickedTile = tilesVect[mouseRow][mouseCol];

                        // if the tile has no flag place one and if it has a flag take it away
                        if (!clickedTile._hasFlag && !clickedTile._isRevealed) {
                            tilesVect[mouseRow][mouseCol].placeFlag(Textures.flagTexture);
                            numFlagsPressed++;
                        }
                        if (clickedTile._hasFlag) {
                            tilesVect[mouseRow][mouseCol]._hasFlag = false;
                            numFlagsPressed--;
                        }
                        // check if game has been won. If it has, do game win
                        gameIsWon = gameWin(tilesVect, numRows, numColumns);
                        if (gameIsWon) {
                            faceSprite.setTexture(faceWinTexture);
                        }
                    }

                }

                // if mouse button pressed
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

                    // determines which row and col have been pressed
                    int cellWidth = gameWindow.getSize().x / numColumns;
                    int cellHeight = (gameWindow.getSize().y - 100) / numRows;
                    auto mouseCoord = sf::Mouse::getPosition(gameWindow);

                    // if button pressed on a game tile
                    if (hiddenTileBounds.contains((float) mouseCoord.x, (float) mouseCoord.y) && !gameIsWon &&
                        !pauseIsPressed && !leaderboardIsPressed) {
                        int mouseCol = mouseCoord.x / cellWidth;
                        int mouseRow = mouseCoord.y / cellHeight;
                        Tile clickedTile = tilesVect[mouseRow][mouseCol];

                        // game over
                        if (clickedTile._isMine && !gameOver && !clickedTile._hasFlag && !leaderboardIsPressed) {
                            // sets face to dead and game over to true
                            faceSprite.setTexture(faceLostTexture);
                            gameOver = true;

                            // reveals mines
                            for (int row = 0; row < numRows; ++row) {
                                for (int col = 0; col < numColumns; ++col) {
                                    if (boardArr[row][col] == 9) {
                                        tilesVect[row][col].revealMines(Textures.mineTexture);
                                    }
                                }
                            }

                        }

                            // if a tile with no surrounding mines is clicked
                        else if (clickedTile._numNearbyMines == 0 && !clickedTile._isRevealed && !gameOver &&
                                 !leaderboardIsPressed) {
                            auto revealed = findSurroundingTiles(tilesVect, mouseRow, mouseCol);
                            revealSurroundingTiles(tilesVect, revealed, numRows, numColumns, Textures);
                            int tempNumFlagsPlaced = 0;
                            for (int i = 0; i < numRows; i++) {
                                for (int j = 0; j < numColumns; j++) {
                                    if (tilesVect[i][j]._hasFlag) { tempNumFlagsPlaced++; }
                                }
                            }
                            numFlagsPressed = tempNumFlagsPlaced;
                        }

                            // a tile that is a number is clicked
                        else if (!clickedTile._isRevealed && !clickedTile._hasFlag && !gameOver &&
                                 !leaderboardIsPressed) {
                            setTexture(tilesVect, Textures, mouseRow, mouseCol);
                            tilesVect[mouseRow][mouseCol]._tileSprite.setTexture(Textures.revealedTexture);
                            tilesVect[mouseRow][mouseCol]._isRevealed = true;
                        }

                        // check if game has been won. If it has, do game win
                        gameIsWon = gameWin(tilesVect, numRows, numColumns);
                        if (gameIsWon) {
                            faceSprite.setTexture(faceWinTexture);
                            for (int i=0; i<numRows; i++){
                                for (int j=0; j<numColumns; j++){
                                    if (!tilesVect[i][j]._hasFlag && tilesVect[i][j]._isMine){
                                        tilesVect[i][j].placeFlag(Textures.flagTexture);
                                        numFlagsPressed = numMines;
                                    }
                                }
                            }
                        }
                    }

                    // debug button pressed reveal all mines
                    if (debugBounds.contains((float) mouseCoord.x, (float) mouseCoord.y) && !gameIsWon &&
                        !pauseIsPressed) {
                        debugIsPressed = !debugIsPressed;
                        for (int row = 0; row < numRows; ++row) {
                            for (int col = 0; col < numColumns; ++col) {
                                if (boardArr[row][col] == 9) {
                                    tilesVect[row][col].revealMines(Textures.mineTexture);
                                }
                            }
                        }
                    }

                    // happy button pressed restart game
                    if (faceBounds.contains((float) mouseCoord.x, (float) mouseCoord.y)) {
                        debugIsPressed = false;
                        gameOver = false;
                        gameIsWon = false;
                        pauseIsPressed = false;
                        faceSprite.setTexture(happyTexture);
                        totalTimeSecs = -1;
                        numFlagsPressed = 0;

                        // remake board
                        Board board(numRows, numColumns, numMines);
                        board.placeMines();
                        board.addNumNearbyMines();
                        board.printBoard();
                        boardArr = board.getBoard();

                        // create vector of tiles
                        tilesVect.clear();
                        for (int row = 0; row < numRows; row++) {
                            vector<Tile> temp;
                            for (int col = 0; col < numColumns; col++) {
                                Tile newTile(col * tileWidth, row * tileHeight, row, col);
                                newTile._numNearbyMines = boardArr[row][col];
                                if (boardArr[row][col] == 9) { newTile._isMine = true; }
                                temp.push_back(newTile);
                            }
                            tilesVect.push_back(temp);
                        }
                    }

                    // leaderboard button is pressed
                    if (leaderboardBounds.contains((float) mouseCoord.x, (float) mouseCoord.y) &&
                        !leaderboardIsPressed) {
                        leaderboardIsPressed = true;

                        hiddenTilesprite.setTexture(revealedTexture);
                        gameWindow.draw(hiddenTilesprite);
                        gameWindow.display();

                        sf::RenderWindow leaderboardWindow(
                                sf::VideoMode(leaderboardWindowWidth, leaderboardWindowHeight), "Leaderboard",
                                sf::Style::Close);

                        while (leaderboardWindow.isOpen()) {
                            leaderboardIsPressed = true;
                            sf::Event leaderboardEvent{};
                            Leaderboard leaderboard;
                            if (!gameIsWon) {
                                if (ind == 0) {
                                    leaderboard.readFile();
                                    string namesString = leaderboard.getStringOfLeaders();
                                    createText(namesText, font, namesString, 18, namesXpos, namesYpos, true, false);
                                }
                            } else {
                                if (ind == 0) {
                                    ind++;
                                    leaderboard.readFile();
                                    leaderboard.addNewPlayer(playerName, totalTimeSecs);
                                    string namesString = leaderboard.getStringOfLeaders();
                                    leaderboard.writeLeaders();
                                    createText(namesText, font, namesString, 18, namesXpos, namesYpos, true, false);
                                }
                            }
                            while (leaderboardWindow.pollEvent(leaderboardEvent)) {
                                // close window
                                if (leaderboardEvent.type == sf::Event::Closed) {
                                    leaderboardWindow.close();
                                    leaderboardIsPressed = false;
                                    ind = 0;
                                }
                            }
                            leaderboardWindow.clear(sf::Color::Blue);
                            leaderboardWindow.draw(leaderboardText);
                            leaderboardWindow.draw(namesText);
                            leaderboardWindow.display();
                        }
                    }

                    // pause button is pressed
                    if (pauseBounds.contains((float) mouseCoord.x, (float) mouseCoord.y) && !gameIsWon && !gameOver &&
                        !leaderboardIsPressed) {
                        pauseIsPressed = !pauseIsPressed;

                    }
                    if (pauseIsPressed) {
                        debugIsPressed = false;
                        hiddenTilesprite.setTexture(revealedTexture);
                        pauseSprite.setTexture(playTexture);
                    }
                    if (!pauseIsPressed) {
                        hiddenTilesprite.setTexture(hiddenTexture);
                        pauseSprite.setTexture(pauseTexture);

                    }

                }
            }

            if (gameIsWon){gameWindow.clear(sf::Color::Green);}
            else if (gameOver){gameWindow.clear(sf::Color::Red);}
            else{gameWindow.clear(sf::Color::White);}

            // gameWindow.clear(sf::Color::White);

            //// timer
            digitSprites[0].setPosition(tensMinPos, digitYPos);
            gameWindow.draw(digitSprites[0]);
            digitSprites[0].setPosition(onesMinPos, digitYPos);
            gameWindow.draw(digitSprites[0]);
            digitSprites[0].setPosition(tensSecPos, digitYPos);
            gameWindow.draw(digitSprites[0]);
            digitSprites[0].setPosition(onesSecPos, digitYPos);
            gameWindow.draw(digitSprites[0]);
            if (counter > -1) {
                if (counter % 2 == 0) { evenTime = (int) clock.getElapsedTime().asSeconds(); }
                else { oddTime = (int) clock.getElapsedTime().asSeconds(); }

                // change numbers
                if (evenTime != oddTime && !pauseIsPressed && !leaderboardIsPressed && !gameOver && !gameIsWon) {
                    totalTimeSecs += 1;
                    mins = totalTimeSecs / 60;
                    secs = totalTimeSecs % 60;
                }
                counter++;
                // minutes
                if (mins < 10) {
                    digitSprites[mins].setPosition(onesMinPos, digitYPos);
                    gameWindow.draw(digitSprites[mins]);
                } else {
                    digitSprites[mins / 10].setPosition(tensMinPos, digitYPos);
                    gameWindow.draw(digitSprites[mins / 10]);
                    digitSprites[mins % 10].setPosition(onesMinPos, digitYPos);
                    gameWindow.draw(digitSprites[mins]);
                }
                // seconds
                if (secs < 10) {
                    digitSprites[secs].setPosition(onesSecPos, digitYPos);
                    gameWindow.draw(digitSprites[secs]);
                } else {
                    digitSprites[secs / 10].setPosition(tensSecPos, digitYPos);
                    gameWindow.draw(digitSprites[secs / 10]);
                    digitSprites[secs % 10].setPosition(onesSecPos, digitYPos);
                    gameWindow.draw(digitSprites[secs % 10]);
                }
            }
            //// end timer

            // flag counter
            int numFlagsRemaining = numFlags - numFlagsPressed;
            if (numFlagsRemaining < 0) {
                digitSprites[10].setPosition(12, digitYPos);
                gameWindow.draw(digitSprites[10]);
                numFlagsRemaining *= -1;
                negativeFlags = true;
            }
            int onesDigit = numFlagsRemaining % 10;
            numFlagsRemaining /= 10;
            int tensDigit = numFlagsRemaining % 10;
            numFlagsRemaining /= 10;
            int hundredsDigit = numFlagsRemaining % 10;
            digitSprites[hundredsDigit].setPosition(33, digitYPos);
            gameWindow.draw(digitSprites[hundredsDigit]);
            digitSprites[tensDigit].setPosition(33 + 21, digitYPos);
            gameWindow.draw(digitSprites[tensDigit]);
            digitSprites[onesDigit].setPosition(33 + 21 + 21, digitYPos);
            gameWindow.draw(digitSprites[onesDigit]);

            // draw all things needed in the window
            gameWindow.draw(hiddenTilesprite);
            gameWindow.draw(faceSprite);
            gameWindow.draw(debugSprite);
            gameWindow.draw(pauseSprite);
            gameWindow.draw(leaderboardSprite);
            for (int i = 0; i < numRows; ++i) {
                for (int j = 0; j < numColumns; j++) {
                    if (tilesVect[i][j]._isRevealed && !pauseIsPressed) { tilesVect[i][j].drawClickedTile(gameWindow); }
                    if (tilesVect[i][j]._hasFlag && !pauseIsPressed) { tilesVect[i][j].drawFlag(gameWindow); }
                    if (tilesVect[i][j]._isMine && (debugIsPressed || gameOver)) {
                        tilesVect[i][j].drawMines(gameWindow);
                    }
                }
            }
            gameWindow.display();
        }
    }


    return 0;
}
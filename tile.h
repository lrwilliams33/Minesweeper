#pragma once
#include <iostream>
#include<vector>
#include <SFML/Graphics.hpp>
using namespace std;

struct Tile{
    int _numNearbyMines = 0;
    bool _isRevealed = false;
    bool _isMine = false;
    bool _hasFlag = false;
    float _posX;
    float _posY;
    int _row;
    int _col;

    sf::Sprite _tileSprite;
    sf::Sprite _numSprite;
    sf::Sprite _mineSprite;
    sf::Sprite _flagSprite;

    vector<Tile*> _surroundingTiles;

    // constructor
    Tile(float posX, float posY, int row, int col){
        _row = row;
        _col = col;
        _posX = posX;
        _posY = posY;
        _tileSprite.setPosition(posX, posY);
        _numSprite.setPosition(posX, posY);
    }

    // reveals tile if it has no number
    void revealEmptyTile(sf::Texture& revealedTileTexture){
        _isRevealed = true;
        _hasFlag = false;
        _tileSprite.setTexture(revealedTileTexture);
    }

    // reveals all mines for debug
    void revealMines(sf::Texture& mineTexture){
        _mineSprite.setTexture(mineTexture);
        _mineSprite.setPosition(_posX, _posY);
    }

    // places a flag when right click
    void placeFlag(sf::Texture& flagTexture){
        _flagSprite.setTexture(flagTexture);
        _flagSprite.setPosition(_posX, _posY);
        _hasFlag = true;
    }

    // draws the sprite
    void drawClickedTile(sf::RenderWindow& window) const{
        window.draw(_tileSprite);
        window.draw(_numSprite);
    }

    // draw mines for debug
    void drawMines(sf::RenderWindow& window) const{
        window.draw(_mineSprite);
    }

    // draws the flag
    void drawFlag(sf::RenderWindow& window) const{
        window.draw(_flagSprite);
    }
};

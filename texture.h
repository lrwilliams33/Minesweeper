#pragma once
#include <SFML/Graphics.hpp>

struct TextureManager{

    sf::Texture flagTexture;
    sf::Texture mineTexture;
    sf::Texture hiddenTexture;
    sf::Texture revealedTexture;
    sf::Texture oneTexture;
    sf::Texture twoTexture;
    sf::Texture threeTexture;
    sf::Texture fourTexture;
    sf::Texture fiveTexture;
    sf::Texture sixTexture;
    sf::Texture sevenTexture;
    sf::Texture eightTexture;

    sf::Texture clockZero;
    sf::Texture clockOne;
    sf::Texture clockTwo;
    sf::Texture clockThree;
    sf::Texture clockFour;
    sf::Texture clockFive;
    sf::Texture clockSix;
    sf::Texture clockSeven;
    sf::Texture clockEight;
    sf::Texture clockNine;
    sf::Texture negativeSign;

    void loadTextures(){

        flagTexture.loadFromFile("files/images/flag.png");
        mineTexture.loadFromFile("files/images/mine.png");
        hiddenTexture.loadFromFile("files/images/tile_hidden.png");
        revealedTexture.loadFromFile("files/images/tile_revealed.png");
        oneTexture.loadFromFile("files/images/number_1.png");
        twoTexture.loadFromFile("files/images/number_2.png");
        threeTexture.loadFromFile("files/images/number_3.png");
        fourTexture.loadFromFile("files/images/number_4.png");
        fiveTexture.loadFromFile("files/images/number_5.png");
        sixTexture.loadFromFile("files/images/number_6.png");
        sevenTexture.loadFromFile("files/images/number_7.png");
        eightTexture.loadFromFile("files/images/number_8.png");

        clockZero.loadFromFile("files/images/digits.png", sf::IntRect(0, 0, 21, 32));
        clockOne.loadFromFile("files/images/digits.png", sf::IntRect(21, 0, 21, 32));
        clockTwo.loadFromFile("files/images/digits.png", sf::IntRect(21*2, 0, 21, 32));
        clockThree.loadFromFile("files/images/digits.png", sf::IntRect(21*3, 0, 21, 32));
        clockFour.loadFromFile("files/images/digits.png", sf::IntRect(21*4, 0, 21, 32));
        clockFive.loadFromFile("files/images/digits.png", sf::IntRect(21*5, 0, 21, 32));
        clockSix.loadFromFile("files/images/digits.png", sf::IntRect(21*6, 0, 21, 32));
        clockSeven.loadFromFile("files/images/digits.png", sf::IntRect(21*7, 0, 21, 32));
        clockEight.loadFromFile("files/images/digits.png", sf::IntRect(21*8, 0, 21, 32));
        clockNine.loadFromFile("files/images/digits.png", sf::IntRect(21*9, 0, 21, 32));
        negativeSign.loadFromFile("files/images/digits.png", sf::IntRect(21*10, 0, 21, 32));
    }

    vector<sf::Texture> digitsVect() const{
        vector<sf::Texture> digitsVector;
        digitsVector.push_back(clockZero);
        digitsVector.push_back(clockOne);
        digitsVector.push_back(clockTwo);
        digitsVector.push_back(clockThree);
        digitsVector.push_back(clockFour);
        digitsVector.push_back(clockFive);
        digitsVector.push_back(clockSix);
        digitsVector.push_back(clockSeven);
        digitsVector.push_back(clockEight);
        digitsVector.push_back(clockNine);
        digitsVector.push_back(negativeSign);
        return digitsVector;
    }

};

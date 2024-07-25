#include <iostream
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;

struct card {
    sf::Texture cardsTexture;
    sf::Texture backOfCardTexture;
    sf::Sprite cardsSprite;
    float scaleX;
    float scaleY;
    float positionX;
    float positionY;
    bool isFlipped = false;

    card(){
        backOfCardTexture.loadFromFile("Cards/reverse.png");
    }

    card(string fileName){
        cardsTexture.loadFromFile(fileName);
        cardsSprite.setTexture(cardsTexture);
        backOfCardTexture.loadFromFile("Cards/reverse.png");

    }

    card(string fileName, float scaleX, float scaleY, float posX, float posY){
        cardsTexture.loadFromFile(fileName);
        cardsSprite.setTexture(cardsTexture);
        backOfCardTexture.loadFromFile("Cards/reverse.png");
        this->scaleX = scaleX;
        this->scaleY = scaleY;
        this->positionX = posX;
        this->positionY = posY;
        cardsSprite.scale(scaleX, scaleY);
        cardsSprite.setPosition(posX, posY);
    }

    void drawCard(sf::RenderWindow& window){ // must pass window by reference
        if(isFlipped == true){
            cardsSprite.setTexture(backOfCardTexture);
        }
        if(isFlipped == false){
            cardsSprite.setTexture(cardsTexture);
        }

        window.draw(cardsSprite);

    }


};

int pseudoMain(){
    card threeOfClubs("Card/3_of_clubs.png");
    card threeOfSpades("Card/3_of_clubs.png", .5, .5, 100, 100);
    window.draw(threeOfClubs.cardsSprite)

    auto cardBounds = twoOfClubs.getGlobalBounds();
    if(cardBounds.contains(mouseX, mouseY)){
        // change texture
    }



    window.clear();
    twoOfClubs.drawCard(window);
    window.display();

}
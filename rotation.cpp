#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

TextureManager g_textureManager;

struct player
{
    sf::Vector2f direction{ 0, 1};
    sf::Vector2f velocity{ 0, 0};
    sf::Vector2f position{ 100, 100};
    sf::Sprite sprite;
};

int main()
{   
    // Create the main window
    sf::RenderWindow window(sf::VideoMode( 800, 600), "SFML window");
    printf("Window created\r\n");
    TextureManager::GetTexture("allSprites_default.png"); 
    printf("Sprite loaded\r\n");
    float angle = 0.0;
    sf::Clock renderTimer;

    player tank_one;
    tank_one.sprite = TextureManager::GetSprite("allSprites_default.png", "tankBody_green_outline.png");
    tank_one.sprite.setPosition(100, 100);
    tank_one.sprite.setOrigin( sf::Vector2f(21, 23));

    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            switch(event.type){
            case sf::Event::Closed:
                window.close();
                break;
            }
        }

        if (renderTimer.getElapsedTime().asMilliseconds() > 40)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) angle += 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) angle -= 1;

            renderTimer.restart();
            // Clear screen
            window.clear();
            // Draw the sprite
            window.draw(sprite);
            // Draw the string
            //window.draw(text);
            // Update the window
            sprite.setRotation(angle);
            //angle += 0.1;
            //if(angle >= 360)
            //    angle = 0.0;
            // get the current mouse position in the window.
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            // convert it to world coordinates.
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

            window.display();
        }
    }
    return EXIT_SUCCESS;
}
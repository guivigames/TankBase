#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

TextureManager g_textureManager;

int main()
{   
    // Create the main window
    sf::RenderWindow window(sf::VideoMode( 800, 600), "SFML window");
    printf("Window created\r\n");
    TextureManager::GetTexture("allSprites_default.png");
    sf::Sprite sprite(TextureManager::GetSprite("allSprites_default.png", "tankBody_green_outline.png")); 
    sprite.setPosition(100, 100);
    sprite.setOrigin( sf::Vector2f(21, 23));
    printf("Sprite loaded\r\n");
    float angle = 0.0;
    sf::Clock renderTimer;
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (renderTimer.getElapsedTime().asMilliseconds() > 40)
        {
            renderTimer.restart();
            // Clear screen
            window.clear();
            // Draw the sprite
            window.draw(sprite);
            // Draw the string
            //window.draw(text);
            // Update the window
            sprite.rotate(1);
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
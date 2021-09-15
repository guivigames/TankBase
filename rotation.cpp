#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <cmath>
TextureManager g_textureManager;

struct player
{
    sf::Vector2f direction{ 0, 1};
    sf::Vector2f velocity{ 0, 0};
    sf::Vector2f position{ 100, 100};
    sf::Sprite sprite;
};

sf::Vector2f angle2direction(float angle)
{
    sf::Vector2f _direction{ sin(-angle*M_PI/180), cos(-angle*M_PI/180)};
    return _direction;
}

int main()
{   
    // Create the main window
    sf::RenderWindow window(sf::VideoMode( 800, 600), "SFML window");
    printf("Window created\r\n");
    TextureManager::GetTexture("allSprites_default.png"); 
    printf("Sprite loaded\r\n");
    float _angle = 0.0;
    sf::Clock renderTimer;

    player tank_one;
    tank_one.position = {100, 100};
    tank_one.sprite = TextureManager::GetSprite("allSprites_default.png", "tankBody_green_outline.png");
    tank_one.sprite.setPosition(tank_one.position);
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
            renderTimer.restart(); ///< Restart timer;

            /// Move the tank.
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) _angle -= 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) _angle += 1;

            float _velocity = 0;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) _velocity += 1.0;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) _velocity -= 1.0;

            tank_one.direction = angle2direction(_angle);
            tank_one.position = tank_one.position + (tank_one.direction * _velocity);

            /// Render Game
            // Clear screen
            window.clear();
            // Draw the sprite
            window.draw(tank_one.sprite);
            // Draw the string
            //window.draw(text);
            // Update the window
            tank_one.sprite.setRotation(_angle);
            tank_one.sprite.setPosition(tank_one.position);
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
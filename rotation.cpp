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

struct bullet
{
    sf::Vector2f direction{ 0, 1};
    float velocity{ 75};
    sf::Vector2f position{ 100, 100};
    sf::Sprite sprite;
    float angle;
};

sf::Vector2f angle2direction(float angle)
{
    sf::Vector2f _direction{ sin(-angle*M_PI/180), cos(-angle*M_PI/180)};
    return _direction;
}

bool isOut( bullet b)
{
    if(b.position.x <= 0 || b.position.y <= 0 || b.position.y >= 800 || b.position.x >= 1442)
    {
        //std::cout << "Bullet out!" << std::endl;
        return true;
    }
    return false;
}

int main()
{   
    // Create the main window
    sf::RenderWindow window(sf::VideoMode( 1422, 800), "SFML window");
    printf("Window created\r\n");
    TextureManager::GetTexture("allSprites_default.png"); 
    printf("Sprite loaded\r\n");
    float _angle = 0.0;
    sf::Clock renderTimer;
    sf::Clock _fpsClock;
    player tank_one;
    std::vector<bullet> _bullets;
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
            case sf::Event::KeyPressed: ///< Add a bullet
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    bullet shot;
                    shot.sprite = TextureManager::GetSprite( "allSprites_default.png", "bulletGreen1_outline.png");
                    shot.sprite.setOrigin(4, 7);
                    shot.angle = _angle;
                    shot.direction = tank_one.direction;
                    shot.velocity = { 75};
                    shot.position = tank_one.position;
                    _bullets.push_back(shot);
                }
                break;
            }
        }

        sf::Time _fpsTimer = _fpsClock.restart();  ///< Frame rate controller.
        float _fpsCoef =_fpsTimer.asSeconds();

        renderTimer.restart(); ///< Restart timer;

        ///< Update all the asets. ////////////////////////////////////////////////////////
        /// Move the tank.
        float _velocity = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) _angle -= (1 *_fpsCoef*50) ;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) _angle += (1*_fpsCoef*50);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) _velocity += (1.0*_fpsCoef*50);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) _velocity -= (1.0*_fpsCoef*50);
        tank_one.direction = angle2direction(_angle);
        tank_one.position = tank_one.position + (tank_one.direction * _velocity);

        ///< update the bullet position
        for (auto& shot : _bullets)
        {
            shot.position = shot.position + (shot.direction * shot.velocity * _fpsCoef);
        }
        ///< check if any bullet is out of scope and remove it if so.
        auto new_end = std::remove_if(_bullets.begin(), _bullets.end(), isOut);
        _bullets.erase(new_end,  _bullets.end());
        /// Render Game ////////////////////////////////////////////////////////////////
        // Clear screen
        window.clear();
        ///< render the player.
        tank_one.sprite.setRotation(_angle);
        tank_one.sprite.setPosition(tank_one.position);
        window.draw(tank_one.sprite);
        
        // Draw the string
        //window.draw(text);
        // Update the window
        ///< Render all bullets.
        for (auto& shot : _bullets)
        {
            shot.sprite.setRotation(shot.angle);
            shot.sprite.setPosition(shot.position);
            window.draw(shot.sprite);
        }
        //angle += 0.1;
        //if(angle >= 360)
        //    angle = 0.0;
        // get the current mouse position in the window.
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        // convert it to world coordinates.
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

        window.display();
    }
    return EXIT_SUCCESS;
}
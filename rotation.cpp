#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <cmath>
#include "collisions.h"

#ifndef M_PI
const float M_PI = 3.1415;
#endif

const int WIDTH     = 1442;
const int HEIGHT    = 800;

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
    if(b.position.x <= 0 || b.position.y <= 0 || b.position.y >= HEIGHT || b.position.x >= WIDTH)
    {
        //std::cout << "Bullet out!" << std::endl;
        return true;
    }
    return false;
}

void addtank(std::vector<player>& badlist)
{
    player bad;
    bad.position = {    static_cast<float>(400 + static_cast<int>(rand()% ((int)WIDTH-500))), 
                        static_cast<float>(400 + static_cast<int>(rand()% ((int)HEIGHT-500)))};
    bad.sprite = TextureManager::GetSprite("allSprites_default.png", "tankBody_green_outline.png");
    bad.sprite.setPosition(bad.position);
    bad.sprite.setOrigin( sf::Vector2f(21, 23));
    badlist.push_back(bad);
}

int main()
{   
    srand( time(nullptr));
    // Create the main window
    sf::RenderWindow window(sf::VideoMode( WIDTH, HEIGHT), "SFML window");
    printf("Window created\r\n");
    TextureManager::GetTexture("allSprites_default.png"); 
    printf("Sprite loaded\r\n");
    float _angle = 0.0;
    sf::Clock renderTimer;
    sf::Clock _fpsClock;
    player tank_one;
    std::vector<player> tank_bad;
    std::vector<bullet> _bullets;

    addtank(tank_bad);
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
        //for (auto shot = _bullets.begin(); shot != _bullets.end(); shot++)

        bool _killed = false;
        for (int i = 0; i < _bullets.size(); i++)
        {
            //(*shot).position = (*shot).position + ((*shot).direction * (*shot).velocity * _fpsCoef);
            _bullets[i].position = _bullets[i].position + (_bullets[i].direction * _bullets[i].velocity * _fpsCoef);
            sf::FloatRect rfShot = _bullets[i].sprite.getGlobalBounds();
            std::vector<sf::Vector2f> vect1;
            vect1.push_back( sf::Vector2f{rfShot.left, rfShot.top});
            vect1.push_back( sf::Vector2f{rfShot.left, rfShot.top + rfShot.height});
            vect1.push_back( sf::Vector2f{rfShot.left + rfShot.width, rfShot.top + rfShot.height});
            vect1.push_back( sf::Vector2f{rfShot.left + rfShot.width, rfShot.top});
            for (auto t = tank_bad.begin(); t != tank_bad.end(); /*t++*/){
                std::vector<sf::Vector2f> vect2;
                sf::FloatRect frTank = (*t).sprite.getGlobalBounds();
                vect2.push_back( sf::Vector2f{frTank.left, frTank.top});
                vect2.push_back( sf::Vector2f{frTank.left, frTank.top + frTank.height});
                vect2.push_back( sf::Vector2f{frTank.left + frTank.width, frTank.top + frTank.height});
                vect2.push_back( sf::Vector2f{frTank.left + frTank.width, frTank.top});
                if (polyPoly(vect1, vect2))
                {
                    _bullets.erase(_bullets.begin() + i);
                    t = tank_bad.erase(t);
                    _killed = true;          
                }
                else {
                    t++;
                }
            }
        }
        if (_killed)
            addtank(tank_bad);
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
        
        ///< Render the bad boys.
        for (auto& t : tank_bad){
            t.sprite.setPosition(t.position);
            window.draw(t.sprite);
        }

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
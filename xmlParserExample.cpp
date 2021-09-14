#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

struct sections 
{
    char name[64];
    int x;
    int y; 
    int width;
    int height;
};

int main()
{
    std::ifstream ifile;
    ifile.open("allSprites_default.xml");
    
    if (!ifile.is_open()){
        std::cout << "Could not open XML file" << std::endl;
        exit(1);
    }

    std::vector<sections> _images;
    std::string _line;
    while ( std::getline(ifile, _line))
    {
        sections _temp;
        if ( sscanf(_line.c_str(), "\t<SubTexture name=\"%[^\"]\" x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\"/>", 
                                    _temp.name, &_temp.x, &_temp.y, &_temp.width, &_temp.height) == 5)
        {
            _images.push_back(_temp);
        }
    }
    std::cout << "Number of SubTextures in the file: " << _images.size() << std::endl;
    ///////////////////////////////////////////////////////////////////////////////////////////

    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("allSprites_default.png"))
        return EXIT_FAILURE;
    sf::Vector2u _size = texture.getSize();

    // Create the main window
    sf::RenderWindow window(sf::VideoMode( _size.x, _size.y), "SFML window");

    sf::Sprite sprite(texture);
    
    // Create a graphical text to display
    //sf::Font font;
    //if (!font.loadFromFile("arial.ttf"))
    //    return EXIT_FAILURE;
    //sf::Text text("Hello SFML", font, 50);
    // Load a music to play//
    //sf::Music music;
    //if (!music.openFromFile("nice_music.ogg"))
    //    return EXIT_FAILURE;
    // Play the music
    //music.play();
    // Start the game loop
    
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
        // Clear screen
        window.clear();
        // Draw the sprite
        window.draw(sprite);
        // Draw the string
        //window.draw(text);
        // Update the window

        // get the current mouse position in the window.
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        // convert it to world coordinates.
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

        for( auto i : _images)
        {   
            sf::RectangleShape rectangle;
            rectangle.setSize(sf::Vector2f(i.width, i.height));
            rectangle.setFillColor(sf::Color::Transparent);
            rectangle.setOutlineColor(sf::Color::Red);
            rectangle.setOutlineThickness(1);
            rectangle.setPosition( i.x, i.y);
            window.draw(rectangle);
            if (sf::Mouse::isButtonPressed( sf::Mouse::Button::Left))
                if (worldPos.x > i.x && worldPos.y > i.y && worldPos.x < (i.x+i.width) && worldPos.y < (i.y+i.height))
                {
                    std::cout << i.name << " Size: " << i.width << " by "<< i.height <<std::endl;
                    sf::sleep(sf::milliseconds(300));
                }
        }

        window.display();
    }
    return EXIT_SUCCESS;
}
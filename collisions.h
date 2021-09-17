#include <vector>
#include <SFML/Graphics.hpp>

bool polyPoly(std::vector<sf::Vector2f> p1, std::vector<sf::Vector2f> p2) ;
bool polyLine(std::vector<sf::Vector2f> vertices, float x1, float y1, float x2, float y2);
bool lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
bool polyPoint(std::vector<sf::Vector2f> vertices, float px, float py);


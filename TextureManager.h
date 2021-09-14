#pragma once
#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

struct sections 
{
    char filename[64] = {0};
    char name[64] = {0};
    int x = 0;
    int y = 0; 
    int width = 0;;
    int height = 0;
};

class TextureManager 
{
protected:
    static TextureManager* m_s_Instance;
    std::map<std::string, sf::Texture> m_Textures;
    std::vector<sections> m_section;
public:
    TextureManager();
    static sf::Texture& GetTexture(std::string const &filename);
    static sf::Sprite GetSprite(std::string const &filename, const std::string sprite);
};

typedef TextureManager TheTextureManager;

#endif
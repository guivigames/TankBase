#include "TextureManager.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <cstring>

TextureManager* TextureManager::m_s_Instance = nullptr;

TextureManager::TextureManager()
{
    assert(m_s_Instance == nullptr);
    m_s_Instance = this;
}

sf::Texture& TheTextureManager::GetTexture(std::string const& filename)
{
    printf("Initial Loading Texture\r\n");
    auto& m = m_s_Instance->m_Textures;
    auto keyValuePair = m.find(filename);
    if (keyValuePair != m.end()) 
    {
        return keyValuePair->second;
    }
    else 
    {
        printf("Loading Texture\r\n");
        auto& texture = m[filename];
        texture.loadFromFile(filename);

        printf("Opening xml file\r\n");
        std::ifstream ifile("allSprites_default.xml");
        
        if (!ifile.is_open()){
            std::cout << "Could not open XML file" << std::endl;
            return texture;
        }
        
        auto& section = m_s_Instance->m_section;
        std::string _line;
        while ( std::getline(ifile, _line))
        {
            sections _temp;
            if ( sscanf(_line.c_str(), "\t<SubTexture name=\"%[^\"]\" x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\"/>", 
                                        _temp.name, &_temp.x, &_temp.y, &_temp.width, &_temp.height) == 5)
            {
                for (int i = 0; i < filename.size(); i++)
                    _temp.filename[i] = filename[i];
                section.push_back(_temp);
            }
        }

        return texture;
    }
}

sf::Sprite TextureManager::GetSprite(std::string const &filename, const std::string sprite)
{
     auto& m = m_s_Instance->m_Textures;
    auto keyValuePair = m.find(filename);
    if (keyValuePair == m.end()) 
    {
        sf::Sprite ret;
        return ret;
    }
    else {
        auto& texture = m[filename];
        auto section  = m_s_Instance->m_section;
        for (auto i : section)
        {
            if ( !std::strcmp(filename.c_str(), i.filename) && !std::strcmp( sprite.c_str(), i.name))
            {
                sf::Sprite sprite( texture, sf::IntRect(i.x, i.y, i.width, i.height));
                return sprite;
            }
        }
    }
    sf::Sprite ret;
    return ret;
}


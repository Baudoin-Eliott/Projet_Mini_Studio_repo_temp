#include "Texture.h"

#include "SFML/Graphics.hpp"

Texture::Texture(const char* _path)
{
    m_texture = new sf::Texture;
    if (!m_texture->loadFromFile(_path))
        throw std::runtime_error("Can't load texture !");

    sf::Vector2u sizes = m_texture->getSize();
    m_width = sizes.x;
    m_height = sizes.y;
}
Texture::~Texture()
{
    delete m_texture;
};
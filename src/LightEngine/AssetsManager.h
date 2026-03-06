#pragma once
#include <unordered_map>

#include "Texture.h"

#include <string>
#include <memory>


    class TextureManager
    {

    private:
        std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
        


        TextureManager() = default;

        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;
    public:

        static TextureManager& getInstance() {
            static TextureManager instance;
            return instance;
        }

        std::shared_ptr<Texture> addTexture(std::string _name, const char* _path)
        {
            if (m_textures.find(_name) != m_textures.end()) {
                return m_textures[_name];
            }
            std::shared_ptr<Texture> texture = std::shared_ptr<Texture>(new Texture(_path));
            m_textures[_name] = texture;

            return m_textures[_name];
        }

        std::shared_ptr<Texture> getTexture(std::string _name) {

            if (m_textures.find(_name) == m_textures.end()) {
                return nullptr;
            }
            return m_textures[_name];
        }



        ~TextureManager() {
            m_textures.clear();
        }
    };
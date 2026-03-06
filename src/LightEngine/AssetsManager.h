#pragma once
#include <unordered_map>

#include "Texture.h"

#include <string>
#include <memory>


class AssetsManager
{

private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;



	AssetsManager() = default;

	AssetsManager(const AssetsManager&) = delete;
	AssetsManager& operator=(const AssetsManager&) = delete;
public:

	static AssetsManager& getInstance() {
		static AssetsManager instance;
		return instance;
	}

	std::shared_ptr<Texture> addTexture(std::string _name, const char* _path);

	std::shared_ptr<Texture> getTexture(std::string _name);


	~AssetsManager() {
		m_textures.clear();
	}
};
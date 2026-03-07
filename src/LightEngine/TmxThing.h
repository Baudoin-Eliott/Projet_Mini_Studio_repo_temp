#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

struct TmxLayer {
	int id;
	int width;
	int height;
	std::string name;
	std::vector<int> tiles;
};

struct TmxObject {
	int id;
	std::string name;
	float x;
	float y;
	float width;
	float height;
};

struct TmxObjectGroup {
	int id;
	std::string name;
	std::vector<TmxObject> objects;
};

struct TmxTileset {
	int firstGid;
	std::string path;
	int tileWidth;
	int tileHeight;
	int columns;
};


struct TmxMap : public sf::Drawable
{
	int width = 0;
	int height = 0;
	int tileWidth = 0;
	int tileHeight = 0;

	std::vector<TmxLayer>       layers;
	std::vector<TmxObjectGroup> objectGroup;
	std::vector<TmxTileset>     tileset;

	bool load(const std::string& path);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	struct RenderLayer
	{
		sf::VertexArray vertices;
		int tilesetIndex;
	};

	std::vector<sf::Texture>   m_textures;
	std::vector<RenderLayer>   m_renderLayers;
};

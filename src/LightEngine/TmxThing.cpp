#include "TmxThing.h"
#include "TmxParser.h"
#include <climits>
#include <iostream>

bool TmxMap::load(const std::string& path)
{
	std::string fullpath = "../../../";
	fullpath += path;
	auto result = TmxParser::load(fullpath);
	if (!result)
	{
		std::cerr << "failed to load: " << path << "\n";
		return false;
	}

	// copie les donnees
	*static_cast<TmxMap*>(this) = std::move(*result);

	// charge une texture par tileset
	for (const TmxTileset& ts : tileset)
	{
		sf::Texture tex;
		if (!tex.loadFromFile(ts.path))
		{
			std::cerr << "failed to load texture: " << ts.path << "\n";
			return false;
		}
		m_textures.push_back(std::move(tex));
	}

	// construit un VertexArray par (layer x tileset)
	for (const TmxLayer& layer : layers)
	{
		for (int tsIdx = 0; tsIdx < (int)tileset.size(); tsIdx++)
		{
			const TmxTileset& ts = tileset[tsIdx];
			int nextFirstGid = (tsIdx + 1 < (int)tileset.size()) ? tileset[tsIdx + 1].firstGid : INT_MAX;

			RenderLayer rl;
			rl.vertices.setPrimitiveType(sf::Quads);
			rl.tilesetIndex = tsIdx;

			for (int y = 0; y < layer.height; y++)
			{
				for (int x = 0; x < layer.width; x++)
				{
					int gid = layer.tiles[y * layer.width + x];
					if (gid == 0 || gid < ts.firstGid || gid >= nextFirstGid)
						continue;

					int localId = gid - ts.firstGid;
					int col = localId % ts.columns;
					int row = localId / ts.columns;

					float px = (float)(x * tileWidth);
					float py = (float)(y * tileHeight);
					float pw = (float)tileWidth;
					float ph = (float)tileHeight;

					float u = (float)(col * ts.tileWidth);
					float v = (float)(row * ts.tileHeight);
					float uw = (float)ts.tileWidth;
					float uh = (float)ts.tileHeight;

					sf::Vertex v0, v1, v2, v3;
					v0.position = { px, py }; v0.texCoords = { u, v };
					v1.position = { px + pw, py }; v1.texCoords = { u + uw, v };
					v2.position = { px + pw, py + ph }; v2.texCoords = { u + uw, v + uh };
					v3.position = { px, py + ph }; v3.texCoords = { u, v + uh };

					rl.vertices.append(v0);
					rl.vertices.append(v1);
					rl.vertices.append(v2);
					rl.vertices.append(v3);
				}
			}

			if (rl.vertices.getVertexCount() > 0)
				m_renderLayers.push_back(std::move(rl));
		}
	}

	return true;
}

void TmxMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const RenderLayer& rl : m_renderLayers)
	{
		states.texture = &m_textures[rl.tilesetIndex];
		target.draw(rl.vertices, states);
	}
}

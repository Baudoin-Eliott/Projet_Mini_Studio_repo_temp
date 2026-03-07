#include "TmxParser.h"
#include <filesystem>
#include <tinyxml2.h>
#include <sstream>
#include <iostream>


static std::vector<int> parseCSV(std::string entry)
{
    std::vector<int> result;
    std::stringstream ss(entry);
    std::string token;
    while (std::getline(ss, token, ','))
    {
        if (token.empty())
            continue;

        bool hasDigit = false;
        for (char c : token)
        {
            if (std::isdigit(c))
            {
                hasDigit = true;
                break;
            }
        }

        if (hasDigit)
        {

            try
            {
                int value = std::stoi(token);
                result.push_back(value);
            }
            catch (const std::exception& e)
            {
                std::cerr << "[TiledParser] error parsing number:" << token << "\n";
            }
        }
    }

    return result;
}

std::optional<TmxMap> TmxParser::load(const std::string& _path)
{
    // on prend le parent l'extension
    std::filesystem::path basePath = std::filesystem::path(_path).parent_path();

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(_path.c_str()) != tinyxml2::XML_SUCCESS)
        return std::nullopt;

    tinyxml2::XMLElement* map = doc.FirstChildElement("map");

    if (!map)
        return std::nullopt;
    TmxMap tmxMap;
    tmxMap.height = map->IntAttribute("height");
    tmxMap.width = map->IntAttribute("width");
    tmxMap.tileHeight = map->IntAttribute("tileheight");
    tmxMap.tileWidth = map->IntAttribute("tilewidth");

    // on gere les tileset
    tinyxml2::XMLElement* currentTileset = map->FirstChildElement("tileset");
    while (currentTileset)
    {
        auto tileset = loadTileSet(basePath.string(), currentTileset);
        if (tileset)
            tmxMap.tileset.push_back(*tileset);
        currentTileset = currentTileset->NextSiblingElement("tileset");
    }

    // on gere les layers (c'est pareil)
    tinyxml2::XMLElement* currentLayer = map->FirstChildElement("layer");
    while (currentLayer)
    {
        auto layer = loadLayer(currentLayer);
        if (layer)
            tmxMap.layers.push_back(*layer);
        currentLayer = currentLayer->NextSiblingElement("layer");
    }

    // obj groupe (pareil)
    tinyxml2::XMLElement* currentObjectGroup = map->FirstChildElement("objectgroup");
    while (currentObjectGroup)
    {
        TmxObjectGroup objGroup = loadObjectGroup(currentObjectGroup);

        tmxMap.objectGroup.push_back(objGroup);
        currentObjectGroup = currentObjectGroup->NextSiblingElement("objectgroup");
    }

    return tmxMap;
}

std::optional<TmxTileset> TmxParser::loadTileSet(const std::string& _path, tinyxml2::XMLElement* _elem)
{
    TmxTileset tileset;
    tileset.firstGid = _elem->IntAttribute("firstgid");
    if (const char* tsxSource = _elem->Attribute("source"))
    {

        std::string fullPath = (std::filesystem::path(_path) / tsxSource).string();
        tinyxml2::XMLDocument tsxDoc;

        if (tsxDoc.LoadFile(fullPath.c_str()) != tinyxml2::XML_SUCCESS)
            return std::nullopt;

        tinyxml2::XMLElement* tsxTileset = tsxDoc.FirstChildElement("tileset");
        if (!tsxTileset)
            return std::nullopt;

        tileset.tileWidth = tsxTileset->IntAttribute("tilewidth");
        tileset.tileHeight = tsxTileset->IntAttribute("tileheight");
        tileset.columns = tsxTileset->IntAttribute("columns");

        tinyxml2::XMLElement* image = tsxTileset->FirstChildElement("image");
        if (!image)
            return tileset;

        const char* imageSource = image->Attribute("source");
        if (!imageSource)
            return tileset;

        tileset.path = (std::filesystem::path(_path) / imageSource).string();
    }
    else
    {

        tileset.tileWidth = _elem->IntAttribute("tilewidth");
        tileset.tileHeight = _elem->IntAttribute("tileheight");
        tileset.columns = _elem->IntAttribute("columns");

        tinyxml2::XMLElement* image = _elem->FirstChildElement("image");
        if (!image)
            return std::nullopt;

        const char* imageSource = image->Attribute("source");
        if (!imageSource)
            return std::nullopt;

        tileset.path = (std::filesystem::path(_path) / imageSource).string();
    }

    return tileset;
}

std::optional<TmxLayer> TmxParser::loadLayer(tinyxml2::XMLElement* _elem)
{
    TmxLayer layer;

    if (const char* name = _elem->Attribute("name"))
        layer.name = name;

    layer.width = _elem->IntAttribute("width");
    layer.height = _elem->IntAttribute("height");
    layer.id = _elem->IntAttribute("id");

    tinyxml2::XMLElement* data = _elem->FirstChildElement("data");
    if (!data)
        return std::nullopt;

    const char* encoding = data->Attribute("encoding");
    if (!encoding || std::string(encoding) != "csv")
        return std::nullopt;

    std::string intData = data->GetText();

    layer.tiles = parseCSV(intData);

    if (layer.tiles.size() == 0)
        return std::nullopt;

    return layer;
}

TmxObject TmxParser::loadObject(tinyxml2::XMLElement* _elem)
{

    TmxObject obj;
    if (_elem->Attribute("name"))
        obj.name = _elem->Attribute("name");
    obj.x = _elem->FloatAttribute("x");
    obj.y = _elem->FloatAttribute("y");
    obj.width = _elem->FloatAttribute("width");
    obj.height = _elem->FloatAttribute("height");
    obj.id = _elem->IntAttribute("id");


    return obj;
}

TmxObjectGroup TmxParser::loadObjectGroup(tinyxml2::XMLElement* _elem)
{
    TmxObjectGroup objGroup;
    if (_elem->Attribute("name"))
        objGroup.name = _elem->Attribute("name");

    objGroup.id = _elem->IntAttribute("id");
    tinyxml2::XMLElement* obj = _elem->FirstChildElement("object");
    while (obj)
    {
        TmxObject object = loadObject(obj);
        objGroup.objects.push_back(object);
        obj = obj->NextSiblingElement("object");
    }

    return objGroup;
}

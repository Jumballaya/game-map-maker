#ifndef TILEMAPLOADER_H
#define TILEMAPLOADER_H

#include <memory>
#include <string>
#include <sstream>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../../libs/pugixml/pugixml.hpp"
#include "../../libs/glm/glm.hpp"

#include "./TileMap.h"
#include "../assetstore/AssetStore.h"

#include "../logger/Logger.h"

class TileMapLoader
{
public:
    TileMapLoader() = default;
    ~TileMapLoader() = default;

    static std::shared_ptr<TileMap> loadTileMap(const std::string &filePath, std::unique_ptr<AssetStore> &assetStore, SDL_Renderer *renderer)
    {
        pugi::xml_document doc;
        auto result = doc.load_file(filePath.c_str());
        if (!result)
        {
            std::cout << "XML [" + filePath + "] parsed with errors\n";
            std::cout << "Error description: " << result.description() << "\n";
            std::cout << "Error offset: " << result.offset << std::endl;
        }
        auto mapNode = doc.child("map");
        int mapWidthInTiles = mapNode.attribute("width").as_int();
        int mapHeightInTiles = mapNode.attribute("height").as_int();
        int tileSize = mapNode.attribute("tileSize").as_int();

        std::shared_ptr<TileMap> tileMap = std::make_shared<TileMap>(glm::vec2(mapWidthInTiles, mapHeightInTiles), tileSize, 1.0);

        // Setup tilesets
        auto tilesets = mapNode.child("tilesets");
        for (pugi::xml_node tileset = tilesets.child("tileset"); tileset; tileset = tileset.next_sibling("tileset"))
        {
            std::string name = tileset.attribute("name").as_string();
            std::string source = tileset.attribute("source").as_string();
            int tileSize = tileset.attribute("tileSize").as_int();
            int cols = tileset.attribute("columns").as_int();
            int rows = tileset.attribute("rows").as_int();
            int width = tileset.attribute("width").as_int();
            int height = tileset.attribute("height").as_int();

            SDL_Surface *surface = IMG_Load(source.c_str());
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            assetStore->addTileset(name, texture, glm::vec2(cols, rows), glm::vec2(width, height), tileSize);
            tileMap->tilesets.push_back(name);
        }

        // Set up layers
        auto layers = mapNode.child("layers");
        std::vector<int> data;
        for (pugi::xml_node layer = layers.child("layer"); layer; layer = layer.next_sibling("layer"))
        {
            std::string name = layer.attribute("name").as_string();
            std::string tilesetName = layer.attribute("tileset").as_string();
            int width = layer.attribute("width").as_int();
            int height = layer.attribute("height").as_int();
            size_t layerId = tileMap.get()->createLayer(name, tilesetName);
            auto dataNode = layer.child("tiledata");
            std::string tiledata = dataNode.child_value();
            auto tileSet = assetStore->getTileset(tilesetName);

            std::string temp;
            std::stringstream stream{tiledata};

            while (std::getline(stream, temp, ','))
            {
                data.push_back(std::stoi(temp));
            };

            int i = 0;
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    int index = data[i];
                    int srcCol = 0;
                    int srcRow = 0;
                    if (index == 0)
                    {
                        srcCol = -1;
                        srcRow = -1;
                    }
                    else
                    {
                        index -= 1;
                        srcCol = index % static_cast<int>(tileSet->sizeTile.x);
                        srcRow = static_cast<int>(std::floor(index / tileSet->sizeTile.x));
                    }
                    tileMap.get()->updateTile(layerId, glm::vec2(x, y), glm::vec2(srcCol, srcRow));
                    i++;
                }
            }

            data.clear();
        }

        return tileMap;
    }

    static void saveTileMap(const std::string &filePath, std::shared_ptr<TileMap>)
    {
        // Convert tilemap into XML
        // Save XML file
    }
};

#endif
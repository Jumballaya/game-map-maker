#ifndef CANVAS_H
#define CANVAS_H

#include <cmath>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

class Canvas
{
public:
  Canvas(int x = 0, int y = 0, int tileSize = 16)
  {
    posX = x;
    posY = y;
    width = 0;
    height = 0;
    this->tileSize = tileSize;
    this->lightColor = {77, 77, 77};
    this->darkColor = {44, 44, 44};
  };
  ~Canvas() = default;

  const int &getWidth() const { return width; }
  const int &getHeight() const { return height; }
  const int &getXPosition() const { return posX; }
  const int &getYPosition() const { return posY; }
  const int &getTileSize() const { return tileSize; }

  const glm::vec2 getTileCoords(glm::vec2 position, double zoom)
  {
    int xIn = static_cast<int>(position.x / zoom);
    int yIn = static_cast<int>(position.y / zoom);
    int xCanvas = static_cast<int>((posX - ((width * zoom) / 2)) / zoom);
    int yCanvas = static_cast<int>((posY - ((height * zoom) / 2)) / zoom);
    glm::vec2 offset = glm::vec2(xIn - xCanvas, yIn - yCanvas);
    if (offset.x < 0 || offset.y < 0)
    {
      return glm::vec2(-1, -1);
    }

    int x = static_cast<int>(std::floor((offset.x / tileSize)));
    int y = static_cast<int>(std::floor((offset.y / tileSize)));
    return glm::vec2(x, y);
  };

  SDL_Rect getRect(double zoom)
  {
    int x = static_cast<int>(posX - ((width * zoom) / 2.0));
    int y = static_cast<int>(posY - ((height * zoom) / 2.0));
    int w = static_cast<int>(width * zoom);
    int h = static_cast<int>(height * zoom);
    return {x, y, w, h};
  }

  void setBackgroundColors(SDL_Color light, SDL_Color dark)
  {
    this->lightColor = light;
    this->darkColor = dark;
  }

  void setTileSize(int tileSize) { this->tileSize = tileSize; }
  void setWidth(int w) { width = w; }
  void setHeight(int h) { height = h; }
  void setPosition(int x, int y)
  {
    posX = x;
    posY = y;
  }

  void offset(int x, int y)
  {
    posX += x;
    posY += y;
  }

  void draw(SDL_Renderer *renderer, double zoom)
  {
    int yStart = static_cast<int>(posY - ((height * zoom) / 2));
    int xStart = static_cast<int>(posX - ((width * zoom) / 2));
    int tileZoom = static_cast<int>(tileSize * zoom);
    for (int y = 0; y < height / tileSize; y++)
    {
      int yPos = yStart + (y * tileZoom);
      for (int x = 0; x < width / tileSize; x++)
      {
        int xPos = xStart + (x * tileZoom);
        SDL_Rect checkerRect = {xPos, yPos, tileZoom, tileZoom};
        if ((x + y) % 2 == 0)
        {
          SDL_SetRenderDrawColor(renderer, darkColor.r, darkColor.g, darkColor.b, 255);
        }
        else
        {
          SDL_SetRenderDrawColor(renderer, lightColor.r, lightColor.g, lightColor.b, 255);
        }
        SDL_RenderFillRect(renderer, &checkerRect);
      }
    }
  };

private:
  int width;
  int height;
  int tileSize;

  int posX;
  int posY;

  SDL_Color lightColor;
  SDL_Color darkColor;
};

#endif
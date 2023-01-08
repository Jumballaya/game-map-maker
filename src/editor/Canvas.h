#ifndef CANVAS_H
#define CANVAS_H

#include <SDL2/SDL.h>

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
    int yStart = posY - ((height * zoom) / 2);
    int xStart = posX - ((width * zoom) / 2);
    for (int y = 0; y < height / tileSize; y++)
    {
      int yPos = yStart + (y * tileSize * zoom);
      for (int x = 0; x < width / tileSize; x++)
      {
        int xPos = xStart + (x * tileSize * zoom);
        SDL_Rect checkerRect = {xPos, yPos, tileSize * zoom, tileSize * zoom};
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
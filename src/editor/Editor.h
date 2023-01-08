#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <SDL2/SDL.h>

#include "../eventbus/EventBus.h"
#include "../assetstore/AssetStore.h"

const int FPS = 30;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Editor
{
public:
  Editor();
  ~Editor();

  // Start/Close
  void initialize();
  void destroy();

  // Editor Loop
  void run();
  void setup();
  void processInput();
  void update();
  void render();
  void renderGui();

  // Editor
  static int windowWidth;
  static int windowHeight;

private:
  SDL_Window *window;
  SDL_Renderer *renderer;

  SDL_Texture *selectedTileset;
  int tileRow;
  int tileCol;
  int tileSize;

  int mapWidth;
  int mapHeight;
  int imgWidth;
  int imgHeight;

  int mouseX;
  int mouseY;

  double zoom;

  std::unique_ptr<AssetStore> assetStore;
  std::unique_ptr<EventBus> eventBus;

  int millisPreviousFrame = 0;

  bool running;
  bool isDebug;
};

#endif
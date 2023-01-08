#include <fstream>

#include <SDL2/SDL_image.h>
#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/imgui_sdl.h"
#include "../../libs/imgui/imgui_impl_sdl.h"

#include "Editor.h"
#include "../logger/Logger.h"
#include "../events/Events.h"

int Editor::windowWidth;
int Editor::windowHeight;

void Editor::renderGui()
{
  ImGui::NewFrame();

  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration;
  int sideBarWidth = imgWidth * 2 + (tileSize / 2);
  ImGui::SetNextWindowSize(ImVec2(sideBarWidth, Editor::windowHeight - 20), 0);
  ImGui::SetNextWindowPos(ImVec2(Editor::windowWidth - sideBarWidth, 20));
  ImGui::Begin("Tiles and Textures", NULL, windowFlags);
  ImGui::Text("Tile Selection", 22);
  ImGui::Separator();
  ImGui::BeginChild("tiles", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

  auto scrollY = ImGui::GetScrollY();
  auto scrollX = ImGui::GetScrollX();

  int imageWidth = imgWidth * 2;
  int imageHeight = imgHeight * 2;

  ImGui::Image(selectedTileset, ImVec2(imageWidth, imageHeight));

  int mousePosX = static_cast<int>(ImGui::GetMousePos().x - ImGui::GetWindowPos().x + scrollX);
  int mousePosY = static_cast<int>(ImGui::GetMousePos().y - ImGui::GetWindowPos().y + scrollY);

  mouseX = ImGui::GetMousePos().x;
  mouseY = ImGui::GetMousePos().y;

  int rows = imageHeight / (tileSize * 2);
  int cols = imageWidth / (tileSize * 2);

  for (int i = 0; i < cols; i++)
  {
    for (int j = 0; j < rows; j++)
    {
      // auto drawList = ImGui::GetWindowDrawList();

      // Check to see if we are in the area of the desired 2D tile
      if ((mousePosX >= (imageWidth / cols) * i && mousePosX <= (imageWidth / cols) + ((imageWidth / cols) * i)) && (mousePosY >= (imageHeight / rows) * j && mousePosY <= (imageHeight / rows) + ((imageHeight / rows) * j)))
      {
        if (ImGui::IsItemHovered())
        {

          if (ImGui::IsMouseClicked(0))
          {

            tileCol = i;
            tileRow = j;
          }
        }
      }
    }
  }

  ImGui::EndChild();

  ImGui::End();

  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("Open Map"))
      {
      }
      if (ImGui::MenuItem("Save Map"))
      {
      }
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  ImGui::ShowDemoWindow();

  ImGui::Render();
  ImGuiSDL::Render(ImGui::GetDrawData());
}

Editor::Editor()
{
  Logger::Log("[Editor] constructor called");
  running = false;
  assetStore = std::make_unique<AssetStore>();
  eventBus = std::make_unique<EventBus>();
}

Editor::~Editor()
{
  Logger::Log("[Editor] destructor called");
}

void Editor::initialize()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    Logger::Error("Error initializing SDL");
    return;
  };

  if (TTF_Init() != 0)
  {
    Logger::Error("Error initializing SDL TTF");
    return;
  }
  SDL_DisplayMode dm;
  SDL_GetCurrentDisplayMode(0, &dm);

  this->windowWidth = dm.w - 10;
  this->windowHeight = dm.h - 10;
  this->window = SDL_CreateWindow(
      "Tile map editor",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      windowWidth,
      windowHeight,
      0);

  if (!window)
  {
    Logger::Error("Error creating SDL window");
    return;
  }

  this->renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer)
  {
    Logger::Error("Error creating SDL renderer");
    return;
  }

  ImGui::CreateContext();
  ImGuiSDL::Initialize(renderer, windowWidth, windowHeight);
  running = true;
}

void Editor::setup()
{
  assetStore->addTexture(renderer, "image", "./assets/f22.png");
  assetStore->addTexture(renderer, "tilemap", "./assets/tiles_packed.png");

  selectedTileset = assetStore->getTexture("tilemap");
  mapWidth = 32;
  mapHeight = 32;
  tileRow = 0;
  tileCol = 0;
  tileSize = 16;
  imgWidth = 12 * 16;
  imgHeight = 10 * 16;
  zoom = 1;
}

void Editor::run()
{
  setup();
  while (running)
  {
    processInput();
    update();
    render();
  }
}

void Editor::processInput()
{
  SDL_Event sdlEvent;
  while (SDL_PollEvent(&sdlEvent))
  {
    // IMGUI SDL input
    ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
    ImGuiIO &io = ImGui::GetIO();

    int mouseX, mouseY;
    const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

    io.MousePos = ImVec2(mouseX, mouseY);
    io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
    io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
    io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
    io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
    io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);

    // core SDL events
    switch (sdlEvent.type)
    {

      // case SDL_WINDOWEVENT:
      //   switch (sdlEvent.window.event)
      //   {
      //   case SDL_WINDOWEVENT_RESIZED:
      //     int w, h;
      //     SDL_GetWindowSize(window, &w, &h);
      //     Editor::windowHeight = h;
      //     Editor::windowWidth = w;
      //     break;
      //   }
      //   break;

    case SDL_QUIT:
      running = false;
      break;

      // case SDL_KEYDOWN:
      //   eventBus->emit<KeyPressEvent>(sdlEvent.key.keysym.sym);
      //   switch (sdlEvent.key.keysym.sym)
      //   {
      //   case SDLK_ESCAPE:
      //     running = false;
      //     break;

      //   case SDLK_BACKSPACE:
      //     io.KeysDown[42] = true;
      //     break;
      //   }
      //   break;

      // case SDL_MOUSEWHEEL:
      //   if (sdlEvent.wheel.y > 0)
      //   {
      //     zoom *= 1.075;
      //   }
      //   if (sdlEvent.wheel.y < 0)
      //   {
      //     zoom *= 0.925;
      //   }
      //   break;
    }
  }
}

void Editor::update()
{
  Uint32 ticks = SDL_GetTicks();
  int timeToWait = MILLISECS_PER_FRAME - (ticks - millisPreviousFrame);
  if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
  {
    SDL_Delay(timeToWait);
  }

  double deltaTime = (ticks - millisPreviousFrame) / 1000.0f;
  this->millisPreviousFrame = ticks;
}

void Editor::render()
{
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  // Render phase
  SDL_Rect rect = {0, 0, windowWidth, windowHeight};
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderFillRect(renderer, &rect);

  // Render the map
  int originX = 50;
  int originY = 50;
  int margin = 2;
  int tileSize = 16;
  double scale = 2 * zoom;
  auto texture = assetStore->getTexture("tilemap");
  for (int y = 0; y < mapHeight; y++)
  {
    int yPos = originY + (y * tileSize * scale) + margin;
    int ysrcRect = y * tileSize;
    for (int x = 0; x < mapWidth; x++)
    {
      int xPos = originX + (x * tileSize * scale) + margin;
      int xSrcRect = x * tileSize;
      // SDL_Rect srcRect = {ysrcRect, xSrcRect, tileSize, tileSize};
      SDL_Rect checkerRect = {xPos, yPos, tileSize * scale, tileSize * scale};
      if ((x + y) % 2 == 0)
      {
        SDL_SetRenderDrawColor(renderer, 44, 44, 44, 255);
      }
      else
      {
        SDL_SetRenderDrawColor(renderer, 77, 77, 77, 255);
      }
      SDL_RenderFillRect(renderer, &checkerRect);
      // SDL_RenderCopy(renderer, texture, &srcRect, &dstrect);
    }
  }

  // Render selected tile at the mouse

  int yPos = mouseY - ((tileSize * scale) / 2);
  int ysrcRect = tileCol * tileSize;
  int xPos = mouseX - ((tileSize * scale) / 2);
  int xSrcRect = tileRow * tileSize;
  SDL_Rect srcRect = {ysrcRect, xSrcRect, tileSize, tileSize};
  SDL_Rect dstrect = {xPos, yPos, tileSize * scale, tileSize * scale};
  SDL_RenderCopy(renderer, selectedTileset, &srcRect, &dstrect);

  renderGui();

  SDL_RenderPresent(renderer);
}

void Editor::destroy()
{
  ImGuiSDL::Deinitialize();
  ImGui::DestroyContext();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
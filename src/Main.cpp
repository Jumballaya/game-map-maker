#include <string>
#include <iostream>

#include "../libs/glm/glm.hpp"

#include "./commands/CommandManager.h"
#include "./commands/TestCommand.h"

#include "./tilemap/TileMap.h"

#include "./editor/Editor.h"
#include "./logger/Logger.h"

int main(int argc, char *argv[])
{
    Editor editor;
    editor.initialize(std::string(argv[0]));
    editor.run();
    editor.destroy();
    return 0;
}

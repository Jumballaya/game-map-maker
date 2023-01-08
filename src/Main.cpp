#include <string>
#include <iostream>

#include "./tilemap/Tilemap.h"

#include "./editor/Editor.h"
#include "./logger/Logger.h"

int main(int argc, char *argv[])
{
    Editor editor;
    editor.initialize();
    editor.run();
    editor.destroy();

    return 0;
}

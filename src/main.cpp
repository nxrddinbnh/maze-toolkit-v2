#include "../include/generate-maze.hpp"
#include "../include/solve-maze.hpp"
#include "../include/tools.hpp"
#include <emscripten.h>
#include <vector>

#define EXTERN extern "C"

std::vector<std::vector<int>> maze;

EXTERN EMSCRIPTEN_KEEPALIVE void initializeMaze(int height, int width)
{
    maze = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));
    Enclose(&maze);
}

EXTERN EMSCRIPTEN_KEEPALIVE void generateMaze(int algorithm)
{
    GenerateMaze(&maze, &algorithm);
}

EXTERN EMSCRIPTEN_KEEPALIVE void solveMaze(int algorithm)
{
    SolveMaze(&maze, &algorithm);
}

EXTERN EMSCRIPTEN_KEEPALIVE int getTypeCell(int y, int x)
{
    return maze[y][x];
}

EXTERN EMSCRIPTEN_KEEPALIVE void setTypeCell(int y, int x, int newType)
{
    maze[y][x] = newType;
}

EXTERN EMSCRIPTEN_KEEPALIVE int getCellOrderSize()
{
    return GetCellOrderSize();
}

EXTERN EMSCRIPTEN_KEEPALIVE void getCellOrder(int index, int *y, int *x)
{
    std::pair<int, int> cell = GetCellOrder(index);
    *y = cell.first;
    *x = cell.second;
}

EXTERN EMSCRIPTEN_KEEPALIVE void
prepareMaze(int type)
{
    if (type == 1)
    {
        Enclose(&maze);
    }
    else if (type == 2)
    {
        Grid(&maze);
    }
    else if (type == 3)
    {
        Fill(&maze);
    }
}

EXTERN EMSCRIPTEN_KEEPALIVE void setEntryExit()
{
    SetEntryExit(&maze);
}

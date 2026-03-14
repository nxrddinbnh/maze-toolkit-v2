#pragma once
#include <vector>
#include <iostream>
#include <random>

namespace CellType {
    constexpr int EMPTY = 0;
    constexpr int WALL  = -1;
    constexpr int ENTRY = 3;
    constexpr int EXIT  = 4;
    constexpr int PATH  = 2147483646;
}

void AddCellToOrder(int y, int x);
int GetCellOrderSize();
std::pair<int, int> GetCellOrder(int index);
void ClearCellOrder();
bool InLimits(std::vector<std::vector<int>> *maze, std::pair<int, int> cell);
std::pair<int, int> RandomCell(std::vector<std::vector<int>> *maze);
void SetEntryExit(std::vector<std::vector<int>> *maze);
std::pair<std::pair<int, int>, std::pair<int, int>> FindEntryExit(std::vector<std::vector<int>> *maze);
void Enclose(std::vector<std::vector<int>> *maze);
void Grid(std::vector<std::vector<int>> *maze);
void Fill(std::vector<std::vector<int>> *maze);
void PrintMaze(std::vector<std::vector<int>> *maze);

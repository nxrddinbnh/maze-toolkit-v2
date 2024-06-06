#pragma once
#include <vector>
#include <iostream>
#include <random>

void CreateGrid(std::vector<std::vector<int>> *maze);
void PrintMaze(std::vector<std::vector<int>> *maze);
void AddPaths(std::vector<std::vector<int>> *maze);
void SetEntryExit(std::vector<std::vector<int>> *maze);

#pragma once
#include <vector>
#include <iostream>
#include <random>

void CreateGrid(std::vector<std::vector<int>> *maze);
void AddPaths(std::vector<std::vector<int>> *maze);
std::pair<int, int> RandomStart(std::vector<std::vector<int>> *maze);
void SetEntryExit(std::vector<std::vector<int>> *maze);
void PrintMaze(std::vector<std::vector<int>> *maze);

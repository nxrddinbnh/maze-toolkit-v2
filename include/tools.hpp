#pragma once
#include <vector>
#include <iostream>
#include <random>

bool InLimits(std::vector<std::vector<int>> *maze, std::pair<int, int> cell);
std::pair<int, int> RandomCell(std::vector<std::vector<int>> *maze);
void SetEntryExit(std::vector<std::vector<int>> *maze);
std::pair<int, int> FindEntryExit(std::vector<std::vector<int>> *maze);
void Enclose(std::vector<std::vector<int>> *maze);
void Grid(std::vector<std::vector<int>> *maze);
void Fill(std::vector<std::vector<int>> *maze);
void PrintMaze(std::vector<std::vector<int>> *maze);

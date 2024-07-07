#include "../../include/generators/kruskal.hpp"
#include "../../include/tools.hpp"

void Kruskal(std::vector<std::vector<int>> *maze)
{
    std::random_device seed;
    std::mt19937 random(seed());

    int indexCell = 0;
    std::vector<std::pair<int, int>> wallList;

    ClearCellOrder();
    Grid(maze);

    // Identify walls and initialize cells
    for (int y = 1; y < (*maze).size() - 1; y++)
    {
        for (int x = 1; x < (*maze)[0].size() - 1; x++)
        {
            if ((*maze)[y][x] == -1)
            {
                if ((y + x) % 2 == 1)
                {
                    wallList.push_back({y, x});
                }
            }
            else
            {
                indexCell++;
                (*maze)[y][x] = indexCell;
            }
        }
    }

    std::shuffle(wallList.begin(), wallList.end(), random);

    for (auto &wall : wallList)
    {
        int y = wall.first;
        int x = wall.second;
        int minValue = -1, maxValue = -1;

        bool directions[2] = {true, false};
        std::shuffle(std::begin(directions), std::end(directions), random);

        for (bool isHorizontal : directions)
        {
            if (isHorizontal)
            {
                if ((*maze)[y][x - 1] != -1 && (*maze)[y][x + 1] != -1 && (*maze)[y][x - 1] != (*maze)[y][x + 1])
                {
                    minValue = std::min((*maze)[y][x - 1], (*maze)[y][x + 1]);
                    maxValue = std::max((*maze)[y][x - 1], (*maze)[y][x + 1]);

                    break;
                }
            }
            else
            {
                if ((*maze)[y - 1][x] != -1 && (*maze)[y + 1][x] != -1 && (*maze)[y - 1][x] != (*maze)[y + 1][x])
                {
                    minValue = std::min((*maze)[y - 1][x], (*maze)[y + 1][x]);
                    maxValue = std::max((*maze)[y - 1][x], (*maze)[y + 1][x]);

                    break;
                }
            }
        }

        if (minValue != -1 && maxValue != -1)
        {
            (*maze)[y][x] = minValue;
            AddCellToOrder(y, x);

            // Change all cells with maxValue to minValue
            for (int i = 1; i < (*maze).size() - 1; i++)
            {
                for (int j = 1; j < (*maze)[0].size() - 1; j++)
                {
                    if ((*maze)[i][j] == maxValue)
                    {
                        (*maze)[i][j] = minValue;
                        AddCellToOrder(i, j);
                    }
                }
            }
        }
    }

    // Convert cells to final maze structure
    for (int y = 1; y < (*maze).size() - 1; y++)
    {
        for (int x = 1; x < (*maze)[0].size() - 1; x++)
        {
            if ((*maze)[y][x] > 0)
            {
                (*maze)[y][x] = 0;
            }
            else
            {
                (*maze)[y][x] = -1;
            }
        }
    }

    SetEntryExit(maze);
}

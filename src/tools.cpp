#include "../include/tools.hpp"

void CreateGrid(std::vector<std::vector<int>> *maze)
{
    for (int height = 0; height < (*maze).size(); height++)
    {
        for (int width = 0; width < (*maze)[0].size(); width++)
        {
            if (height % 2 == 0)
            {
                (*maze)[height][width] = -1;
            }
            else
            {
                if (width % 2 == 0)
                {
                    (*maze)[height][width] = -1;
                }
                else
                {
                    (*maze)[height][width] = 0;
                }
            }
        }
    }
}

void AddPaths(std::vector<std::vector<int>> *maze)
{
    std::random_device seed;
    std::mt19937 random(seed());

    int totalArea = (*maze).size() * (*maze)[0].size();
    double percentageWalls = 0.1;
    int minWalls = totalArea * percentageWalls;
    int maxWalls = totalArea * (percentageWalls * 1.2);

    std::uniform_int_distribution<int> number(minWalls, maxWalls);
    int walls = number(random);

    for (int i = 0; i < walls; i++)
    {
        int randomX = std::uniform_int_distribution<int>(1, (*maze)[0].size() - 2)(random);
        int randomY = std::uniform_int_distribution<int>(1, (*maze).size() - 2)(random);

        if ((*maze)[randomY][randomX] == -1)
        {
            (*maze)[randomY][randomX] = 2;
        }
    }
}

std::pair<int, int> RandomStart(std::vector<std::vector<int>> *maze)
{
    std::random_device seed;
    std::mt19937 random(seed());

    std::uniform_int_distribution<int> distributionX(0, (*maze)[0].size() - 1);
    std::uniform_int_distribution<int> distributionY(0, (*maze).size() - 1);

    int startX, startY;

    do
    {
        startX = distributionX(random);
        startY = distributionY(random);
    } while ((*maze)[startY][startX] == -1);

    return std::make_pair(startY, startX);
}

void SetEntryExit(std::vector<std::vector<int>> *maze)
{
    std::random_device seed;
    std::mt19937 random(seed());

    std::uniform_int_distribution<int> randomEntry(1, (*maze).size() - 2);
    std::uniform_int_distribution<int> randomExit(1, (*maze).size() - 2);
    int entry = randomEntry(random);
    int exit = randomExit(random);

    (*maze)[entry][0] = 3;
    (*maze)[entry][1] = 3;
    (*maze)[exit][(*maze)[0].size() - 1] = 3;
    (*maze)[exit][(*maze)[0].size() - 2] = 3;
}

void PrintMaze(std::vector<std::vector<int>> *maze)
{
    std::cout << std::endl;

    for (int height = 0; height < (*maze).size(); height++)
    {
        for (int width = 0; width < (*maze)[0].size(); width++)
        {
            if ((*maze)[height][width] == -1)
            {
                std::cout << "# "; // Walls
            }
            else if ((*maze)[height][width] == 2147483646)
            {
                std::cout << ". "; // Right way
            }
            else
            {
                std::cout << "  "; // Empty cells
            }
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

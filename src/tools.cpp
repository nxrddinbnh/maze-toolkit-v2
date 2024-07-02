#include "../include/tools.hpp"

bool InLimits(std::vector<std::vector<int>> *maze, std::pair<int, int> cell)
{
    bool inLimits = false;

    if (cell.first > 0 &&
        cell.first < (*maze).size() &&
        cell.second > 0 &&
        cell.second < (*maze)[0].size())
    {
        inLimits = true;
    }

    return inLimits;
}

std::pair<int, int> RandomCell(std::vector<std::vector<int>> *maze)
{
    std::random_device seed;
    std::mt19937 random(seed());

    std::uniform_int_distribution<int> distributionX(1, (*maze)[0].size() - 2);
    std::uniform_int_distribution<int> distributionY(1, (*maze).size() - 2);

    std::pair<int, int> start;

    do
    {
        start = {distributionY(random), distributionX(random)};
    } while (start.first % 2 == 0 || start.second % 2 == 0);

    return start;
}

void SetEntryExit(std::vector<std::vector<int>> *maze)
{
    std::random_device seed;
    std::mt19937 random(seed());

    std::uniform_int_distribution<int> randomEntryExit(1, (*maze).size() - 2);
    int entry, exit;

    do
    {
        entry = randomEntryExit(random);
    } while (entry % 2 == 0);

    do
    {
        exit = randomEntryExit(random);
    } while (exit % 2 == 0);

    (*maze)[entry][0] = 3;
    (*maze)[exit][(*maze)[0].size() - 1] = 3;
}

std::pair<int, int> FindEntryExit(std::vector<std::vector<int>> *maze)
{
    std::pair<int, int> entryExit;

    for (int i = 0; i < (*maze).size(); i++)
    {
        if ((*maze)[i][0] == 3)
        {
            entryExit.first = i;
            break;
        }
    }

    for (int i = 0; i < (*maze).size(); i++)
    {
        if ((*maze)[i][(*maze)[0].size() - 1] == 3)
        {
            entryExit.second = i;
            break;
        }
    }

    return entryExit;
}

void Enclose(std::vector<std::vector<int>> *maze)
{
    for (int i = 0; i < (*maze).size(); i++)
    {
        (*maze)[i][0] = -1;
        (*maze)[i][(*maze)[0].size() - 1] = -1;
    }

    for (int i = 0; i < (*maze)[0].size(); i++)
    {
        (*maze)[0][i] = -1;
        (*maze)[(*maze).size() - 1][i] = -1;
    }
}

void Grid(std::vector<std::vector<int>> *maze)
{
    for (int y = 0; y < (*maze).size(); y++)
    {
        for (int x = 0; x < (*maze)[0].size(); x++)
        {
            if (y % 2 == 0)
            {
                (*maze)[y][x] = -1;
            }
            else
            {
                if (x % 2 == 0)
                {
                    (*maze)[y][x] = -1;
                }
                else
                {
                    (*maze)[y][x] = 0;
                }
            }
        }
    }
}

void Fill(std::vector<std::vector<int>> *maze)
{
    for (int y = 0; y < (*maze).size(); y++)
    {
        for (int x = 0; x < (*maze)[0].size(); x++)
        {
            (*maze)[y][x] = -1;
        }
    }
}

void PrintMaze(std::vector<std::vector<int>> *maze)
{
    std::cout << std::endl;

    for (int y = 0; y < (*maze).size(); y++)
    {
        for (int x = 0; x < (*maze)[0].size(); x++)
        {
            if ((*maze)[y][x] <= -3 || (*maze)[y][x] == -1)
            {
                std::cout << "# "; // Walls
            }
            else if ((*maze)[y][x] == 2147483646)
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

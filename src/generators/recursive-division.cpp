#include "../../include/generators/recursive-division.hpp"
#include "../../include/tools.hpp"

void RecursiveDivision(std::vector<std::vector<int>> *maze)
{
    std::srand(std::time(nullptr));

    int xMax = (*maze)[0].size() - 2;
    int yMax = (*maze).size() - 2;
    int xMin = 1;
    int yMin = 1;

    std::vector<std::pair<int, int>> holes;

    Enclose(maze);

    // Recursive function to divide the maze
    std::function<void(int, int, int, int)> Division = [&](int xMin, int xMax, int yMin, int yMax) 
    {
        // Stop dividing if area is too small
        if (xMax - xMin < 2 || yMax - yMin < 2) return;

        bool isHorizontal;
        if (xMax - xMin >= 3 && yMax - yMin >= 3)
        {
            isHorizontal = rand() % 2 == 0;
        }
        else if (xMax - xMin < 3)
        {
            isHorizontal = true;
        }
        else
        {
            isHorizontal = false;
        }

        if (isHorizontal)
        {
            int y = rand() % ((yMax - 1) - (yMin + 1) + 1) + (yMin + 1);
            int hole = rand() % (xMax - xMin + 1) + xMin;

            // Prevent pathways wider than 1 unit
            if (y % 2 != 0) y++;
            if (y > yMax) y -= 2;

            // Prevent create exits in front of walls
            if (hole % 2 == 0) hole++;
            if (hole > xMax) hole -= 2;

            for (int i = xMin; i <= xMax; i++)
                if (i != hole) (*maze)[y][i] = -1;

            holes.push_back({y, hole});

            Division(xMin, xMax, yMin, y - 1);
            Division(xMin, xMax, y + 1, yMax);
        }
        else
        {
            int x = rand() % ((xMax - 1) - (xMin + 1) + 1) + (xMin + 1);
            int hole = rand() % (yMax - yMin + 1) + yMin;

            // Prevent pathways wider than 1 unit
            if (x % 2 != 0) x++;
            if (x > xMax) x -= 2;

            // Prevent create exits in front of walls
            if (hole % 2 == 0) hole++;
            if (hole > yMax) hole -= 2;

            for (int i = yMin; i <= yMax; ++i)
                if (i != hole) (*maze)[i][x] = -1;

            holes.push_back({hole, x});

            Division(xMin, x - 1, yMin, yMax);
            Division(x + 1, xMax, yMin, yMax);
        }
    };

    Division(xMin, xMax, yMin, yMax);
    SetEntryExit(maze);
}

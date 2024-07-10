#include "../../include/generators/recursive-division.hpp"
#include "../../include/tools.hpp"

void RecursiveDivision(std::vector<std::vector<int>> *maze)
{
    std::srand(std::time(nullptr));

    int maxX = (*maze)[0].size() - 2;
    int maxY = (*maze).size() - 2;
    int minX = 1;
    int minY = 1;

    ClearCellOrder();
    Enclose(maze);

    // Recursive function to divide the maze
    std::function<void(int, int, int, int)> Division = [&](int newMinX, int newMaxX, int newMinY, int newMaxY)
    {
        // Stop dividing if area is too small
        if (newMaxX - newMinX < 2 || newMaxY - newMinY < 2)
        {
            return;
        }

        int numLines;
        bool isHorizontal;

        if (newMinX == minX && newMaxX == maxX && newMinY == minY && newMaxY == maxY)
        {
            numLines = rand() % 4 + 1;
            isHorizontal = false;
        }
        else
        {
            numLines = 1;

            if (newMaxX - newMinX >= 4 && newMaxY - newMinY >= 4)
            {
                isHorizontal = rand() % 2 == 0;
            }
            else if (newMaxX - newMinX < 4)
            {
                isHorizontal = true;
            }
            else
            {
                isHorizontal = false;
            }
        }

        if (isHorizontal)
        {
            int y, hole;
            int lastY = -10;

            for (int i = 0; i < numLines; i++)
            {
                do
                {
                    y = rand() % ((newMaxY - 1) - (newMinY + 1) + 1) + (newMinY + 1);
                } while (y % 2 == 1 || std::abs(y - lastY) < 2);

                lastY = y;

                do
                {
                    hole = rand() % (newMaxX - newMinX + 1) + newMinX;
                } while (hole % 2 == 0);

                // Trace the line of walls
                for (int j = newMinX; j <= newMaxX; j++)
                {
                    if (j != hole)
                    {
                        (*maze)[y][j] = -1;
                        AddCellToOrder(y, j);
                    }
                }
            }

            Division(newMinX, newMaxX, newMinY, y - 1);
            Division(newMinX, newMaxX, y + 1, newMaxY);
        }
        else
        {
            int x, hole;

            do
            {
                x = rand() % ((newMaxX - 1) - (newMinX + 1) + 1) + (newMinX + 1);
            } while (x % 2 == 1);

            do
            {
                hole = rand() % (newMaxY - newMinY + 1) + newMinY;
            } while (hole % 2 == 0);

            // Trace the line of walls
            for (int j = newMinY; j <= newMaxY; j++)
            {
                if (j != hole)
                {
                    (*maze)[j][x] = -1;
                    AddCellToOrder(j, x);
                }
            }

            Division(newMinX, x - 1, newMinY, newMaxY);
            Division(x + 1, newMaxX, newMinY, newMaxY);
        }
    };

    Division(minX, maxX, minY, maxY);
}

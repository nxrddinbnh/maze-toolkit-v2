#include "../../include/generators/binary-tree.hpp"
#include "../../include/tools.hpp"

void BinaryTree(std::vector<std::vector<int>> *maze)
{
    std::srand(std::time(nullptr));

    ClearCellOrder();
    Fill(maze);

    for (int x = 1; x < (*maze)[0].size() - 1; x++)
    {
        (*maze)[1][x] = 0;
        AddCellToOrder(1, x);
    }

    for (int y = 1; y < (*maze).size() - 1; y += 2)
    {
        for (int x = 1; x < (*maze)[0].size() - 1; x += 2)
        {
            (*maze)[y][x] = 0;
            AddCellToOrder(y, x);

            if (x == 1 && y > 1)
            {
                (*maze)[y - 1][x] = 0;
                AddCellToOrder(y - 1, x);
            }
            else if (x > 1 || y > 1)
            {
                if (std::rand() % 2 == 0 && y > 1)
                {
                    (*maze)[y - 1][x] = 0;
                    AddCellToOrder(y - 1, x);
                }
                else if (x > 1)
                {
                    (*maze)[y][x - 1] = 0;
                    AddCellToOrder(y, x - 1);
                }
            }
        }
    }

    SetEntryExit(maze);
}

#include "../../include/generators/side-winder.hpp"
#include "../../include/tools.hpp"

void SideWinder(std::vector<std::vector<int>> *maze)
{
    std::srand(std::time(nullptr));
    std::vector<std::pair<int, int>> setCells;

    ClearCellOrder();
    Fill(maze);

    for (int x = 1; x < (*maze)[0].size() - 1; x++)
    {
        (*maze)[1][x] = 0;
        AddCellToOrder(1, x);
    }

    for (int y = 3; y < (*maze).size(); y += 2)
    {
        for (int x = 1; x < (*maze)[0].size() - 1; x += 2)
        {
            (*maze)[y][x] = 0;
            AddCellToOrder(y, x);
            setCells.push_back({y, x});

            if (std::rand() % 2 == 0 || x == (*maze)[0].size() - 2)
            {
                std::pair<int, int> cell = setCells[std::rand() % setCells.size()];
                (*maze)[cell.first - 1][cell.second] = 0;
                AddCellToOrder(cell.first - 1, cell.second);
                setCells.clear();
            }
            else
            {
                (*maze)[y][x + 1] = 0;
                AddCellToOrder(y, x + 1);
            }
        }
    }
}

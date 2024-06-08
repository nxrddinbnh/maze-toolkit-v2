#include "../../include/solvers/breadth-first.hpp"
#include "../../include/tools.hpp"

void BreadthFirst(std::vector<std::vector<int>> *maze)
{
    int entry, exit;

    std::vector<std::vector<bool>> visited((*maze).size(), std::vector<bool>((*maze)[0].size(), false));
    std::vector<std::vector<std::pair<int, int>>> prev((*maze).size(), std::vector<std::pair<int, int>>((*maze)[0].size(), {-1, -1}));
    std::queue<std::pair<int, int>> queue;
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    // Find the entry and exit
    for (int i = 0; i < (*maze).size(); i++) 
    {
        if ((*maze)[i][0] == 3)
        {
            entry = i;
            break;
        }
    }

    for (int i = 0; i < (*maze).size(); i++) 
    {
        if ((*maze)[i][(*maze)[0].size() - 1] == 3)
        {
            exit = i;
            break;
        }
    }

    queue.push({entry, 0});
    visited[entry][0] = true;

    while (!queue.empty())
    {
        int currentY = queue.front().first;
        int currentX = queue.front().second;
        queue.pop();

        // Check if the exit has been reached
        if (currentX == (*maze)[0].size() - 1 && currentY == exit)
        {
            // Trace back to mark the path
            while (currentX != -1 && currentY != -1)
            {
                (*maze)[currentY][currentX] = 2147483646;
                std::pair<int, int> prevCell = prev[currentY][currentX];
                currentY = prevCell.first;
                currentX = prevCell.second;
            }
            return;
        }

        // Explore neighboring cells
        for (const std::pair<int, int>& dir : directions)
        {
            int nextY = currentY + dir.first;
            int nextX = currentX + dir.second;

            if (nextX >= 0 && nextX < (*maze)[0].size() && nextY >= 0 && nextY < (*maze).size())
            {
                if ((*maze)[nextY][nextX] != -1 && !visited[nextY][nextX])
                {
                    queue.push({nextY, nextX});
                    visited[nextY][nextX] = true;
                    prev[nextY][nextX] = {currentY, currentX};
                }
            }
        }
    }
}

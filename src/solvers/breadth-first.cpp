#include "../../include/solvers/breadth-first.hpp"
#include "../../include/tools.hpp"

void BreadthFirst(std::vector<std::vector<int>> *maze)
{
    std::vector<std::vector<bool>> isVisited((*maze).size(), std::vector<bool>((*maze)[0].size(), false));
    std::vector<std::vector<std::pair<int, int>>> prev((*maze).size(), std::vector<std::pair<int, int>>((*maze)[0].size(), {-1, -1}));
    std::queue<std::pair<int, int>> queue;
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    int entry = FindEntryExit(maze).first;
    int exit = FindEntryExit(maze).second;

    queue.push({entry, 0});
    isVisited[entry][0] = true;

    while (!queue.empty())
    {
        std::pair<int, int> current = queue.front();
        queue.pop();

        // Check if the exit has been reached
        if (current.first == exit && current.second == (*maze)[0].size() - 1)
        {
            // Trace back to mark the path
            while (current.first != -1 && current.second != -1)
            {
                (*maze)[current.first][current.second] = 2147483646;
                std::pair<int, int> prevCell = prev[current.first][current.second];
                current = prevCell;
            }

            return;
        }

        // Explore neighboring cells
        for (const std::pair<int, int> &dir : directions)
        {
            int nextY = current.first + dir.first;
            int nextX = current.second + dir.second;

            if (InLimits(maze, {nextY, nextX}) && (*maze)[nextY][nextX] != -1 && !isVisited[nextY][nextX])
            {
                queue.push({nextY, nextX});
                isVisited[nextY][nextX] = true;
                prev[nextY][nextX] = current;
            }
        }
    }
}

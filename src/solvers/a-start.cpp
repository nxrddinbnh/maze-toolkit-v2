#include "../../include/solvers/a-start.hpp"
#include "../../include/tools.hpp"

void AStar(std::vector<std::vector<int>> *maze)
{
    std::priority_queue<std::pair<int, std::pair<int, int>>, std::vector<std::pair<int, std::pair<int, int>>>, std::greater<std::pair<int, std::pair<int, int>>>> nodeList;
    std::vector<std::vector<bool>> isVisited((*maze).size(), std::vector<bool>((*maze)[0].size(), false));
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    std::vector<std::vector<std::pair<int, int>>> prev((*maze).size(), std::vector<std::pair<int, int>>((*maze)[0].size(), {-1, -1}));
    std::vector<std::vector<int>> tempMaze = *maze;

    ClearCellOrder();
    std::pair<int, int> entry = FindEntryExit(maze).first;
    std::pair<int, int> exit = FindEntryExit(maze).second;

    // Start algorithm
    nodeList.push({0, entry});
    isVisited[entry.first][entry.second] = true;
    AddCellToOrder(entry.first, entry.second);

    while (!nodeList.empty())
    {
        int currentCost = nodeList.top().first;
        std::pair<int, int> current = nodeList.top().second;
        nodeList.pop();

        // If we reach the exit
        if (current == exit)
        {
            // Trace back to mark the shortest path
            while (!(current == entry))
            {
                if ((*maze)[current.first][current.second] != 3)
                {
                    (*maze)[current.first][current.second] = 2147483646;
                }

                AddCellToOrder(current.first, current.second);
                std::pair<int, int> prevCell = prev[current.first][current.second];
                current = prevCell;
            }

            (*maze)[exit.first][exit.second] = 4;
            return;
        }

        // Explore neighboring nodes
        for (auto &dir : directions)
        {
            int nextY = current.first + dir.first;
            int nextX = current.second + dir.second;

            // Check boundaries and if the next cell is passable and not visited
            if (InLimits(maze, {nextY, nextX}) && (*maze)[nextY][nextX] != -1)
            {
                int costFromStart = currentCost + 1;
                int heuristic = std::abs(nextY - exit.first) + std::abs(nextX - exit.second);
                int totalCost = costFromStart + heuristic;

                // Update if found a shorter path or the node is being visited for the first time
                if (!isVisited[nextY][nextX] || totalCost < tempMaze[nextY][nextX])
                {
                    nodeList.push({totalCost, {nextY, nextX}});
                    prev[nextY][nextX] = {current.first, current.second};
                    isVisited[nextY][nextX] = true;
                    tempMaze[nextY][nextX] = totalCost;
                    AddCellToOrder(nextY, nextX);
                }
            }
        }
    }
}

#include "../../include/solvers/a-start.hpp"
#include "../../include/tools.hpp"

void AStar(std::vector<std::vector<int>> *maze)
{
    std::priority_queue<std::pair<int, std::pair<int, int>>,
                        std::vector<std::pair<int, std::pair<int, int>>>,
                        std::greater<std::pair<int, std::pair<int, int>>>> nodeList;
             
    std::vector<std::vector<bool>> isVisited((*maze).size(), std::vector<bool>((*maze)[0].size(), false));
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    std::vector<std::vector<std::pair<int, int>>> prev((*maze).size(), std::vector<std::pair<int, int>>((*maze)[0].size(), {-1, -1}));

    int entry = FindEntryExit(maze).first;
    int exit = FindEntryExit(maze).second;

    // Start algorithm
    nodeList.push({0, {entry, 0}});
    isVisited[entry][0] = true;

    while (!nodeList.empty()) 
    {
        int currentCost = nodeList.top().first;
        int currentY = nodeList.top().second.first;
        int currentX = nodeList.top().second.second;
        nodeList.pop();

        // If we reach the exit
        if (currentX == (*maze)[0].size() - 1 && currentY == exit) 
        {
            // Trace back to mark the shortest path
            while (!(currentY == entry && currentX == 0)) 
            {
                (*maze)[currentY][currentX] = 2147483646;
                std::pair<int, int> prevCell = prev[currentY][currentX];
                currentY = prevCell.first;
                currentX = prevCell.second;
            }

            (*maze)[entry][0] = 2147483646;
            return;
        }

        // Explore neighboring nodes
        for (auto& dir : directions) 
        {
            int nextY = currentY + dir.first;
            int nextX = currentX + dir.second;

            // Check boundaries and if the next cell is passable and not visited
            if (nextX >= 0 
                && nextX < (*maze)[0].size() 
                && nextY >= 0 
                && nextY < (*maze).size()
                && (*maze)[nextY][nextX] != -1) 
            {
                int costFromStart = currentCost + 1;
                int heuristic = abs(nextY - exit) + abs(nextX - ((*maze)[0].size() - 1));
                int totalCost = costFromStart + heuristic;

                // Update if found a shorter path or the node is being visited for the first time
                if (!isVisited[nextY][nextX] || totalCost < (*maze)[nextY][nextX]) 
                {
                    nodeList.push({totalCost, {nextY, nextX}});
                    prev[nextY][nextX] = {currentY, currentX};
                    isVisited[nextY][nextX] = true;
                    (*maze)[nextY][nextX] = totalCost;
                }
            }
        }
    }
}
#include "../../include/solvers/bidirectional-bfs.hpp"
#include "../../include/tools.hpp"

void BidirectionalBFS(std::vector<std::vector<int>> *maze)
{
    int entry = FindEntryExit(maze).first;
    int exit = FindEntryExit(maze).second;

    std::vector<std::vector<bool>> visitedEntry((*maze).size(), std::vector<bool>((*maze)[0].size(), false));
    std::vector<std::vector<bool>> visitedExit((*maze).size(), std::vector<bool>((*maze)[0].size(), false));
    std::vector<std::vector<std::pair<int, int>>> prevEntry((*maze).size(), std::vector<std::pair<int, int>>((*maze)[0].size(), {-1, -1}));
    std::vector<std::vector<std::pair<int, int>>> prevExit((*maze).size(), std::vector<std::pair<int, int>>((*maze)[0].size(), {-1, -1}));

    std::queue<std::pair<int, int>> queueEntry;
    std::queue<std::pair<int, int>> queueExit;

    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    queueEntry.push({entry, 0});
    visitedEntry[entry][0] = true;

    queueExit.push({exit, (*maze)[0].size() - 1});
    visitedExit[exit][(*maze)[0].size() - 1] = true;

    while (!queueEntry.empty() && !queueExit.empty())
    {
        // BFS from the entry
        if (!queueEntry.empty())
        {
            int currentY = queueEntry.front().first;
            int currentX = queueEntry.front().second;
            queueEntry.pop();

            for (const std::pair<int, int> &dir : directions)
            {
                int nextY = currentY + dir.first;
                int nextX = currentX + dir.second;

                // Cell is accessible
                if (nextX >= 0 && nextX < (*maze)[0].size() && nextY >= 0 && nextY < (*maze).size())
                {
                    if ((*maze)[nextY][nextX] != -1 && !visitedEntry[nextY][nextX])
                    {
                        queueEntry.push({nextY, nextX});
                        visitedEntry[nextY][nextX] = true;
                        prevEntry[nextY][nextX] = {currentY, currentX};
                    }
                }
            }
        }

        // BFS from the exit
        if (!queueExit.empty())
        {
            int currentY = queueExit.front().first;
            int currentX = queueExit.front().second;
            queueExit.pop();

            for (const std::pair<int, int> &dir : directions)
            {
                int nextY = currentY + dir.first;
                int nextX = currentX + dir.second;

                // Cell is accessible
                if (nextX >= 0 && nextX < (*maze)[0].size() && nextY >= 0 && nextY < (*maze).size())
                {
                    if ((*maze)[nextY][nextX] != -1 && !visitedExit[nextY][nextX])
                    {
                        queueExit.push({nextY, nextX});
                        visitedExit[nextY][nextX] = true;
                        prevExit[nextY][nextX] = {currentY, currentX};
                    }
                }
            }
        }

        // Check if the two searches have been found
        for (int y = 0; y < (*maze).size(); ++y)
        {
            for (int x = 0; x < (*maze)[0].size(); ++x)
            {
                if (visitedEntry[y][x] && visitedExit[y][x])
                {
                    // Rebuild the road from the entry to the intersection
                    int currentY = y;
                    int currentX = x;

                    while (currentX != -1 && currentY != -1)
                    {
                        (*maze)[currentY][currentX] = 2147483646;
                        std::pair<int, int> prevCell = prevEntry[currentY][currentX];
                        currentY = prevCell.first;
                        currentX = prevCell.second;
                    }

                    // Rebuild the road from the exit to the intersection
                    currentY = y;
                    currentX = x;

                    while (currentX != -1 && currentY != -1)
                    {
                        (*maze)[currentY][currentX] = 2147483646;
                        std::pair<int, int> prevCell = prevExit[currentY][currentX];
                        currentY = prevCell.first;
                        currentX = prevCell.second;
                    }

                    return;
                }
            }
        }
    }
}

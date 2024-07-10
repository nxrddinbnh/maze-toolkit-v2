#include "../../include/solvers/bidirectional-bfs.hpp"
#include "../../include/tools.hpp"

void BidirectionalBFS(std::vector<std::vector<int>> *maze)
{
    std::vector<std::vector<bool>> isVisitedEntry((*maze).size(), std::vector<bool>((*maze)[0].size(), false));
    std::vector<std::vector<bool>> isVisitedExit((*maze).size(), std::vector<bool>((*maze)[0].size(), false));
    std::vector<std::vector<std::pair<int, int>>> prevEntry((*maze).size(), std::vector<std::pair<int, int>>((*maze)[0].size(), {-1, -1}));
    std::vector<std::vector<std::pair<int, int>>> prevExit((*maze).size(), std::vector<std::pair<int, int>>((*maze)[0].size(), {-1, -1}));

    std::queue<std::pair<int, int>> queueEntry;
    std::queue<std::pair<int, int>> queueExit;

    std::vector<std::pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    ClearCellOrder();
    std::pair<int, int> entry = FindEntryExit(maze).first;
    std::pair<int, int> exit = FindEntryExit(maze).second;

    queueEntry.push(entry);
    isVisitedEntry[entry.first][entry.second] = true;
    AddCellToOrder(entry.first, entry.second);

    queueExit.push(exit);
    isVisitedExit[exit.first][exit.second] = true;
    AddCellToOrder(exit.first, exit.second);

    while (!queueEntry.empty() && !queueExit.empty())
    {
        // BFS from the entry
        if (!queueEntry.empty())
        {
            std::pair<int, int> current = queueEntry.front();
            queueEntry.pop();

            for (const std::pair<int, int> &dir : directions)
            {
                int nextY = current.first + dir.first;
                int nextX = current.second + dir.second;

                // Cell is accessible
                if (InLimits(maze, {nextY, nextX}))
                {
                    if ((*maze)[nextY][nextX] != -1 && !isVisitedEntry[nextY][nextX])
                    {
                        queueEntry.push({nextY, nextX});
                        isVisitedEntry[nextY][nextX] = true;
                        prevEntry[nextY][nextX] = current;
                        AddCellToOrder(nextY, nextX);
                    }
                }
            }
        }

        // BFS from the exit
        if (!queueExit.empty())
        {
            std::pair<int, int> current = queueExit.front();
            queueExit.pop();

            for (const std::pair<int, int> &dir : directions)
            {
                int nextY = current.first + dir.first;
                int nextX = current.second + dir.second;

                // Cell is accessible
                if (InLimits(maze, {nextY, nextX}))
                {
                    if ((*maze)[nextY][nextX] != -1 && !isVisitedExit[nextY][nextX])
                    {
                        queueExit.push({nextY, nextX});
                        isVisitedExit[nextY][nextX] = true;
                        prevExit[nextY][nextX] = current;
                        AddCellToOrder(nextY, nextX);
                    }
                }
            }
        }

        // Check if the two searches have been found
        for (int y = 0; y < (*maze).size(); ++y)
        {
            for (int x = 0; x < (*maze)[0].size(); ++x)
            {
                if (isVisitedEntry[y][x] && isVisitedExit[y][x])
                {
                    // Rebuild the road from the entry to the intersection
                    int currentY = y;
                    int currentX = x;

                    while (currentX != -1 && currentY != -1)
                    {
                        if ((*maze)[currentY][currentX] != 3)
                        {
                            (*maze)[currentY][currentX] = 2147483646;
                        }

                        AddCellToOrder(currentY, currentX);
                        std::pair<int, int> prevCell = prevEntry[currentY][currentX];
                        currentY = prevCell.first;
                        currentX = prevCell.second;
                    }

                    // Rebuild the road from the exit to the intersection
                    currentY = y;
                    currentX = x;

                    while (currentX != -1 && currentY != -1)
                    {
                        if ((*maze)[currentY][currentX] != 3)
                        {
                            (*maze)[currentY][currentX] = 2147483646;
                        }

                        AddCellToOrder(currentY, currentX);
                        std::pair<int, int> prevCell = prevExit[currentY][currentX];
                        currentY = prevCell.first;
                        currentX = prevCell.second;
                    }

                    (*maze)[exit.first][exit.second] = 4;
                    return;
                }
            }
        }
    }
}

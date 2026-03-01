#include "Pathfinder.hpp"
#include <algorithm>
#include <set>

namespace rct {

std::vector<std::pair<int, int>> Pathfinder::findPath(int startX, int startY, int endX, int endY, int worldW, int worldH) {
    std::priority_queue<Node*, std::vector<Node*>, NodeCompare> openList;
    std::map<std::pair<int, int>, Node*> closedList;
    std::vector<Node*> allNodes; // For memory cleanup

    Node* startNode = new Node(startX, startY, 0, heuristic(startX, startY, endX, endY));
    openList.push(startNode);
    allNodes.push_back(startNode);

    Node* targetNode = nullptr;

    while (!openList.empty()) {
        Node* current = openList.top();
        openList.pop();

        if (current->x == endX && current->y == endY) {
            targetNode = current;
            break;
        }

        closedList[{current->x, current->y}] = current;

        // Neighboring tiles
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (int i = 0; i < 4; ++i) {
            int nx = current->x + dx[i];
            int ny = current->y + dy[i];

            if (nx < 0 || nx >= worldW || ny < 0 || ny >= worldH) continue;
            if (closedList.count({nx, ny})) continue;

            int ng = current->g + 1;
            int nh = heuristic(nx, ny, endX, endY);
            
            // Note: In a real project, we'd check if another path to this node is better.
            // Simplified for prototype.
            Node* neighbor = new Node(nx, ny, ng, nh, current);
            openList.push(neighbor);
            allNodes.push_back(neighbor);
            closedList[{nx, ny}] = neighbor; // Simplified safety
        }
    }

    std::vector<std::pair<int, int>> path;
    if (targetNode) {
        Node* temp = targetNode;
        while (temp) {
            path.push_back({temp->x, temp->y});
            temp = temp->parent;
        }
        std::reverse(path.begin(), path.end());
    }

    // Cleanup nodes
    for (Node* n : allNodes) delete n;

    return path;
}

} // namespace rct

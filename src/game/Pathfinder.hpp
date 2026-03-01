#ifndef RCT_PATHFINDER_HPP
#define RCT_PATHFINDER_HPP

#include <vector>
#include <queue>
#include <map>
#include <cmath>

namespace rct {

struct Node {
    int x, y;
    int g, h;
    Node* parent;

    Node(int x, int y, int g = 0, int h = 0, Node* parent = nullptr)
        : x(x), y(y), g(g), h(h), parent(parent) {}

    int f() const { return g + h; }
};

struct NodeCompare {
    bool operator()(Node* a, Node* b) {
        return a->f() > b->f();
    }
};

class Pathfinder {
public:
    static std::vector<std::pair<int, int>> findPath(int startX, int startY, int endX, int endY, int worldW, int worldH);

private:
    static int heuristic(int x1, int y1, int x2, int y2) {
        return std::abs(x1 - x2) + std::abs(y1 - y2); // Manhattan distance
    }
};

} // namespace rct

#endif // RCT_PATHFINDER_HPP

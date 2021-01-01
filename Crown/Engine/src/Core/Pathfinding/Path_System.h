#pragma once
#include "Path_Node.h"
#include "Path_Grid.h"
#include <vector>
#include <map>
#include <queue>

template<typename T, typename priority_t>
struct PriorityQueue {
    typedef std::pair<priority_t, T> PQElement;
    std::priority_queue<PQElement, std::vector<PQElement>,
        std::greater<PQElement>> elements;

    inline bool empty() const {
        return elements.empty();
    }

    inline void put(T item, priority_t priority) {
        elements.emplace(priority, item);
    }

    T get() {
        T best_item = elements.top().second;
        elements.pop();
        return best_item;
    }
};

class Path_Grid;

class Path_System
{
public:
    void SetPathGrid(Path_Grid& path_grid);
    std::vector<Path_Node*> FindPath(Path_Node* start,  Path_Node* end);
    std::vector<Path_Node*> FindPath(const glm::vec3& StartPosition, const glm::vec3& EndPosition);
    ~Path_System();
    void ResetNodes();

private:
    std::vector<Path_Node*> PathAlgorithm(Path_Grid& path_grid, Path_Node* start, Path_Node* end);
    void AssignNeighbours(Path_Node* node);
    inline int Heuristic(Path_Node* a, Path_Node* b) const
    {
        return static_cast<int>(abs(a->worldPosition.x - b->worldPosition.x) + abs(a->worldPosition.y - b->worldPosition.y));
    }

    PriorityQueue<Path_Node*, int> Frontier;
    std::map<Path_Node*, Path_Node*> Came_From;
    Path_Node* StartNode, *EndNode, *CurrentNode;
    Path_Grid pathGrid;
    bool EndFound;
};


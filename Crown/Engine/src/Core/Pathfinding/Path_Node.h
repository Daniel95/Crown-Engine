#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class Path_Node
{
public:
    glm::vec3 worldPosition;
    glm::ivec2 gridIndex;
    bool Walkable, Diagonal;
    int NodeCost;
    int CurrentCost;
    


    Path_Node(const bool& walkable, const glm::vec3& pos, const int cost)
    {
        Walkable = walkable;
        worldPosition = pos;
        NodeCost = cost;
        CurrentCost = 100000;
        Diagonal = false;
    }
    Path_Node()
    {
        Walkable = false;
        worldPosition = glm::vec3(0.f,0.f,0.f);
        NodeCost = 0;
        CurrentCost = 100000;
        Diagonal = false;
    }
};


#pragma once
#include "Misc/PathfindingDetails.hpp"
#include <forward_list>
#include <unordered_set>

#define MAP_DIMENSION 40
#define MAP_AREA MAP_DIMENSION*MAP_DIMENSION

#define SQRT2 1.41421356237f


struct Node;

class BucketOpenList
{
    using Bucket = Node*;
    Bucket buckets[MAP_AREA];
    size_t pointer;
public:
    BucketOpenList();
    void Insert(Node* pNode);
    void Remove(Node* pNode);
    bool IsEmpty();
    Node* Pop();
    void Clear();
};

struct Node
{
    Node* next = nullptr;
    Node* parent;
    GridPos pos;
    unsigned char neighbours[8];
    float finalCost;		// Final cost f(x)
    float givenCost;		// Given cost g(x)
    unsigned char nCount;
    //Bottom Right Top Left Bottom Right Top Right Top Left Bottom Left
};

struct Path
{
    bool reachable = false;
    std::vector<short> paths;
    Path()
    {
        paths.reserve(400);
    }
};


class AStarPather
{
    Node map[MAP_AREA];
    BucketOpenList openList;
    byte open[MAP_AREA / 8 + 1];
    byte closed[MAP_AREA / 8 + 1];
    int ADJACENT[8];
    int columns;
    int rows;

    void CallbackMapChanged();
    void PopulateNeighbours(Node* pNode);
    bool TryAddNeighbour(Node* pNode, int row, int col);
    void Rubberband(Node* pStart, Node* pEnd, PathRequest& request);
    void RubberbandAndSmoothing(Node* pStart, Node* pEnd, PathRequest& request);
    bool IsRubberbandable(Node* pStart, Node* pEnd);
    void Smooth(Node* pStart, Node* pEnd, PathRequest& request);

    void AddBackPoints(PathRequest& request);

    static inline float Octile(const GridPos& start, const GridPos& end)
    {
        const int xDiff = std::abs(end.col - start.col);
        const int yDiff = std::abs(end.row - start.row);
        const int minDiff = (xDiff < yDiff) ? xDiff : yDiff;
        const int maxDiff = (xDiff < yDiff) ? yDiff : xDiff;
        return minDiff * SQRT2 + maxDiff - minDiff;
    }

    PathResult SpeedTest(PathRequest& request);
public:
    /* 
        The class should be default constructible, so you may need to define a constructor.
        If needed, you can modify the framework where the class is constructed in the
        initialize functions of ProjectTwo and ProjectThree.
    */

    /* ************************************************** */
    // DO NOT MODIFY THESE SIGNATURES
    bool initialize();
    void shutdown();
    PathResult compute_path(PathRequest &request);

    void FloydWarshall();

    Path pathTable[MAP_AREA][MAP_AREA];

    /* ************************************************** */

    /*
        You should create whatever functions, variables, or classes you need.
        It doesn't all need to be in this header and cpp, structure it whatever way
        makes sense to you.
    */
};

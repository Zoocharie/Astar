#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"

BucketOpenList::BucketOpenList()
{
    //validBuckets.reserve(MAP_AREA/2);
}

bool BucketOpenList::IsEmpty()
{
    return pointer == MAP_AREA;
}

void BucketOpenList::Insert(Node* pNode)
{
    size_t index = size_t(pNode->finalCost);
    if (pointer > index)
    {
        //buckets[index].next = buckets+pointer;
        pointer = index;
    }
    //else
    //{
        //Bucket* bStart = buckets + pointer;
        //while (bStart)
        //{
        //    if (!bStart->next || size_t(bStart->next - buckets) > index)
        //    {
        //        buckets[index].next = bStart->next;
        //        bStart->next = buckets + index;
        //        break;
        //    }
        //}
    //}
    Bucket& bucket = buckets[index];
    pNode->next = bucket;
    bucket = pNode;
    
    //Pointer not cached yet;
    //Node* prev = nullptr;
    //while (start)
    //{
    //    //If node is already lower cost
    //    if (start->finalCost > pNode->finalCost)
    //    {
    //        pNode->next = start;
    //        break;
    //    }
    //    prev = start;
    //    start = start->next;
    //}
    //Starting node
    //if (!prev)
    //    buckets[index] = pNode;
    //else
    //{
    //    prev->next = pNode;
    //}
}

void BucketOpenList::Clear()
{
    for (Bucket& bucket : buckets)
    {
        Node* start = bucket;
        while (start)
        {
            Node* next = start->next;
            start->next = nullptr;
            start = next;
        }
        bucket = nullptr;
    }
    //validBuckets.clear();
    pointer = MAP_AREA - 1;
}

void BucketOpenList::Remove(Node* pNode)
{
    //Index of node in bucket list to be removed
    size_t index = size_t(pNode->finalCost);
    //Starting node of the bucket
    Bucket& bucket = buckets[index];
    Node* start = bucket;
    Node* prev = nullptr;
    while (start)
    {
        //If currentNode == pNode
        if (start == pNode)
        {
            //If previous node exists
            if (prev)
            {
                //Set previous node next to be removed node's next
                prev->next = pNode->next;
            }
            //No previous node, meaning it is a starting node
            else
            {
                //Set starting node to be next of removed node
                bucket = pNode->next;
            }
            pNode->next = nullptr;  // Update next pointer to nullptr
            break;
        }
        prev = start;
        start = start->next;
    }

    //Empty bucket, find next cheapest
    if (!bucket)
    {
        //accessedBuckets.erase(index);
        ///validBuckets.erase(index);
        //ADD PREV NODE
        if (index == pointer)
        {
            while (pointer < MAP_AREA && buckets[pointer] == nullptr)
                ++pointer;
        }
        //if (index == pointer)
        //{
        //    size_t minValue = totalBuckets;

        //    for (const auto& element : validBuckets)
        //    {
        //        if (element < minValue)
        //        {
        //            minValue = element;
        //        }
        //    }
        //    pointer = minValue;
        //}
    }
}

Node* BucketOpenList::Pop()
{
    //Get cheapest bucket
    Bucket& bucket = buckets[pointer];//buckets[pointer];
    //Get first node from bucket
    float cost = 3200.f;
    Node* cheapestNode = bucket; 
    Node* prevCheapestNode = nullptr; // Store the previous node of the cheapest node
    Node* prev = nullptr;
    Node* start = cheapestNode;
    //Find cheapest node
    while (start)
    {
        //If node is already lower cost
        if (start->finalCost < cost)
        {
            cost = start->finalCost;
            cheapestNode = start;
            prevCheapestNode = prev; // Store the previous node of the cheapest node
        }
        prev = start; // Update the previous node
        start = start->next;
    }
    if (prevCheapestNode)
        prevCheapestNode->next = cheapestNode->next;
    else
        bucket = cheapestNode->next; // Update bucket head if the first node is the cheapest
    //Remove it from the bucket
    cheapestNode->next = nullptr;  // Update next pointer to nullptr
    //If left with null pointer, bucket is now empty, find next cheapest bucket
    if (!bucket)
    {
        while (pointer < MAP_AREA && buckets[pointer]== nullptr)
            ++pointer;
    }
    return cheapestNode;
}

bool AStarPather::TryAddNeighbour(Node* pNode, int row, int col)
{
    if (terrain->is_valid_grid_position(row, col) && !terrain->is_wall(row, col))
    {
        //pNode->neighbours[pNode->nCount] = row * columns + col;
        ++pNode->nCount;
        return true;
    }
    return false;
}

void AStarPather::PopulateNeighbours(Node* pNode)
{
    GridPos& pos = pNode->pos;
    bool bottom = false;
    bool right = false;
    bool top = false;

    //Bottom
    if (TryAddNeighbour(pNode, pos.row - 1, pos.col))
    {
        bottom = true;
        pNode->neighbours[pNode->nCount - 1] = 0;
        //pNode->neighboursBitset |= 1;
    }
    //Right
    if (TryAddNeighbour(pNode,pos.row, pos.col + 1))
    {
        right = true;
        pNode->neighbours[pNode->nCount - 1] = 1;
        //pNode->neighboursBitset |= 1 << 1;
        if (bottom && TryAddNeighbour(pNode, pos.row - 1, pos.col + 1))
        {
            pNode->neighbours[pNode->nCount - 1] = 4;
            //pNode->neighboursBitset |= 1 << 4;
        }
    }
    //Top
    if (TryAddNeighbour(pNode,pos.row + 1, pos.col))
    {
        top = true;
        pNode->neighbours[pNode->nCount - 1] = 2;
        //pNode->neighboursBitset |= 1 << 2;
        //TopRight
        if (right && TryAddNeighbour(pNode, pos.row + 1, pos.col + 1))
        {
            pNode->neighbours[pNode->nCount - 1] = 5;
            //pNode->neighboursBitset |= 1 << 5;
        }
    }
    //Left
    if (TryAddNeighbour(pNode,pos.row, pos.col - 1))
    {
        pNode->neighbours[pNode->nCount - 1] = 3;
        //pNode->neighboursBitset |= 1 << 3;
        //BottomLeft
        if (bottom && TryAddNeighbour(pNode, pos.row - 1, pos.col - 1))
        {
            pNode->neighbours[pNode->nCount - 1] = 7;
            //pNode->neighboursBitset |= 1 << 7;
        }
        //TopLeft
        if (top && TryAddNeighbour(pNode, pos.row + 1, pos.col - 1))
        {
            pNode->neighbours[pNode->nCount - 1] = 6;
            //pNode->neighboursBitset |= 1 << 6;
        }
    }
}

#pragma region Extra Credit
bool ProjectTwo::implemented_floyd_warshall()
{
    return true;
}

bool ProjectTwo::implemented_goal_bounding()
{
    return false;
}

bool ProjectTwo::implemented_jps_plus()
{
    return false;
}
#pragma endregion

namespace HeuristicAlgorithm
{
    using HeuristicFunction = float(*)(const GridPos& start, const GridPos& end);

    static inline float Manhatten(const GridPos& start, const GridPos& end)
    {
        float xDiff = fabsf(float(end.col - start.col));
        float yDiff = fabsf(float(end.row - start.row));
        return xDiff + yDiff;
    }

    static inline float Chebyshev(const GridPos& start, const GridPos& end)
    {
        float xDiff = fabsf(float(end.col - start.col));
        float yDiff = fabsf(float(end.row - start.row));
        return xDiff > yDiff ? xDiff : yDiff;
    }

    static inline float Euclidean(const GridPos& start, const GridPos& end)
    {
        float xDiff = float(end.col - start.col);
        float yDiff = float(end.row - start.row);
        return sqrt(xDiff*xDiff + yDiff*yDiff);
    }

    static inline float Octile(const GridPos& start, const GridPos& end)
    {
        const int xDiff = std::abs(end.col - start.col);
        const int yDiff = std::abs(end.row - start.row);
        const int minDiff = (xDiff < yDiff) ? xDiff : yDiff;
        const int maxDiff = (xDiff < yDiff) ? yDiff : xDiff;
        return minDiff * sqrt(2.f) + maxDiff - minDiff;
    }

    static inline float Inconsistent(const GridPos& start, const GridPos& end)
    {
        if ((start.row + start.col) % 2 > 0)
            return Euclidean(start, end);
        return 0;
    }

    HeuristicFunction GetFunction(Heuristic type)
    {
        switch (type)
        {
            case Heuristic::CHEBYSHEV:
                return Chebyshev;
            case Heuristic::MANHATTAN:
                return Manhatten;
            case Heuristic::EUCLIDEAN:
                return Euclidean;
            case Heuristic::OCTILE:
                return Octile;
            case Heuristic::INCONSISTENT:
                return Inconsistent;
        };
        return nullptr;
    }
}

bool AStarPather::initialize()
{
    Callback cb = std::bind(&AStarPather::CallbackMapChanged, this);
    Messenger::listen_for_message(Messages::MAP_CHANGE, cb);
    return true; // return false if any errors actually occur, to stop engine initialization
}

void AStarPather::shutdown()
{
    /*
        Free any dynamically allocated memory or any other general house-
        keeping you need to do during shutdown.
    */
}


bool AStarPather::IsRubberbandable(Node* pStart, Node* pEnd)
{
    if (pStart == nullptr)
        return false;
    const GridPos& pos = pEnd->pos;
    int xDiff = pStart->pos.col - pos.col;
    int yDiff = pStart->pos.row - pos.row;
    int xIncrement = xDiff > 0 ? -1 : 1;
    int yIncrement = yDiff > 0 ? -1 : 1;
    for (int x = xDiff;x!=0;x+=xIncrement)
    {
        for (int y = yDiff; y != 0; y += yIncrement)
        {
            if (terrain->is_wall(pos.row+y,pos.col+x))
                return false;
        }
        if (terrain->is_wall(pos.row, pos.col + x))
            return false;
    }
    for (int y = yDiff; y != 0; y += yIncrement)
    {
        if (terrain->is_wall(pos.row + y,pos.col))
            return false;
    }
    return true;
}

void AStarPather::AddBackPoints(PathRequest& request)
{
    auto startIt = request.path.begin();
    auto nextIt = std::next(startIt);
    auto endIt = request.path.end();
    float GRID_DIST = 1.5f * terrain->mapSizeInWorld / (float)columns;
    --endIt;
    while (startIt != endIt) {
        Vec3 start = *startIt;
        Vec3 next = *nextIt;
        float dist = Vec3::Distance(start, next);
        if (dist > GRID_DIST)
        {
            request.path.insert(nextIt, (next + start) / 2.f);
            startIt = std::prev(startIt);
        }
        ++startIt;
        nextIt = std::next(startIt);
    }
}

void AStarPather::RubberbandAndSmoothing(Node* pStart, Node* pEnd, PathRequest& request)
{
    Rubberband(pStart, pEnd, request);
    AddBackPoints(request);
    //SMOOTH
    auto startIt = request.path.begin();
    auto endIt = std::prev(request.path.end());
    auto p3 = endIt;
    auto p4 = p3;
    auto p2 = std::prev(p3) == startIt ? std::prev(p3) : p3;
    auto p1 = std::prev(p2) == startIt ? std::prev(p2) : p2;

    p4 = p3;
    p3 = p2;
    p2 = p1;
    if (p1 != startIt)
        p1 = std::prev(p1);
    p4 = p3;
    p3 = p2;
    p2 = p1;
    if (p1 != startIt)
        p1 = std::prev(p1);

    while (p3 != startIt)
    {
        //If P4 was the end node or the start Node
        Vec3 v1 = *p1;
        Vec3 v2 = *p2;
        Vec3 v3 = *p3;
        Vec3 v4 = *p4;
        request.path.insert(std::next(p2), Vec3::CatmullRom(v1, v2, v3, v4, 0.75));
        request.path.insert(std::next(p2), Vec3::CatmullRom(v1, v2, v3, v4, 0.5));
        request.path.insert(std::next(p2), Vec3::CatmullRom(v1, v2, v3, v4, 0.25));

        p4 = p3;
        p3 = p2;
        p2 = p1;
        if (p1 != startIt)
            p1 = std::prev(p1);
    }
}

void AStarPather::Rubberband(Node* pStart, Node* pEnd, PathRequest& request)
{
    while (pEnd != pStart)
    {
        while (
            pEnd->parent && 
            pEnd->parent->parent &&
            IsRubberbandable(pEnd->parent->parent, pEnd))
        {
            pEnd->parent = pEnd->parent->parent;
        }
        request.path.push_front
        (terrain->get_world_position(pEnd->pos));
        pEnd = pEnd->parent;
    }
    request.path.push_front
    (terrain->get_world_position(pStart->pos));
}

void AStarPather::Smooth(Node* pStart, Node* pEnd, PathRequest& request)
{
    if (pEnd->parent == pStart)
    {
        request.path.push_front(terrain->get_world_position(pEnd->pos));
        request.path.push_front(terrain->get_world_position(pStart->pos));
        return;
    }


    Node* p3 = pEnd;
    Node* p4 = pEnd;
    Node* p2 = p3->parent ? p3->parent : p3 ;
    Node* p1 = p2->parent ? p2->parent : p2;


    Node* next = pEnd;

    while (next != pStart)
    {
        //If P4 was the end node or the start Node
        Vec3 v1 = terrain->get_world_position(p1->pos);
        Vec3 v2 = terrain->get_world_position(p2->pos);
        Vec3 v3 = terrain->get_world_position(p3->pos);
        Vec3 v4 = terrain->get_world_position(p4->pos);
        request.path.push_front(terrain->get_world_position(next->pos));
        request.path.push_front(Vec3::CatmullRom(v1, v2, v3, v4, 0.75));
        request.path.push_front(Vec3::CatmullRom(v1, v2, v3, v4, 0.5));
        request.path.push_front(Vec3::CatmullRom(v1, v2, v3, v4, 0.25));
        next = next->parent;

        p4 = p3;
        p3 = p2;
        p2 = p1;
        if (p1->parent != nullptr)
            p1 = p1->parent;
    }
    request.path.push_front(terrain->get_world_position(pStart->pos));
}


PathResult AStarPather::SpeedTest(PathRequest& request)
{
    GridPos start = terrain->get_grid_position(request.start);
    GridPos end = terrain->get_grid_position(request.goal);

    Node* pStartNode = map + start.row * columns + start.col;
    Node* pEndNode = map + end.row * columns + end.col;

    memset(open, 0, MAP_AREA / 8 + 1);
    memset(closed, 0, MAP_AREA / 8 + 1);
    openList.Clear();
    pStartNode->givenCost = 0;
    pStartNode->finalCost = Octile(start, end);
    pStartNode->parent = nullptr;
    openList.Insert(pStartNode);
    while (openList.IsEmpty() == false)
    {
        Node* parentNode = openList.Pop();
        if (parentNode == pEndNode)
        {
            while (pEndNode != pStartNode)
            {
                request.path.push_front
                (terrain->get_world_position(pEndNode->pos));
                pEndNode = pEndNode->parent;
            }
            request.path.push_front
            (terrain->get_world_position(pEndNode->pos));
            return PathResult::COMPLETE;
        }
        unsigned short index = unsigned short(parentNode - map);
        closed[index / 8] |= 1 << (index % 8);
        for (int i = 0; i < parentNode->nCount; ++i)
        {
            unsigned char nI = parentNode->neighbours[i];
            int neighbourIndex = index + ADJACENT[nI];
            Node& node = map[neighbourIndex];
            float givenCost = nI < 4 ? parentNode->givenCost + 1 : parentNode->givenCost + SQRT2;
            byte& openBits = open[neighbourIndex / 8];
            int bitsOffset{ 1 << (neighbourIndex % 8) };
            if (!(openBits & bitsOffset) && !(closed[neighbourIndex / 8] & bitsOffset))
            {
                node.parent = parentNode;
                openBits |= bitsOffset;
                node.givenCost = givenCost;
                node.finalCost = givenCost + Octile(node.pos, end) * 1.01f;
                openList.Insert(&node);
            }
            //Already on some list but the cost was higher
            else
            {
                float cost = givenCost + Octile(node.pos, end) * 1.01f;
                if (node.finalCost > cost)
                {
                    //If on the openlist
                    if (openBits & bitsOffset)
                    {
                        openList.Remove(&node);
                    }
                    else
                    {
                        openBits |= bitsOffset;
                    }
                    node.parent = parentNode;
                    node.givenCost = givenCost;
                    node.finalCost = cost;
                    openList.Insert(&node);
                }
            }
        }
    }
    return PathResult::IMPOSSIBLE;
}

void AStarPather::FloydWarshall()
{
    //Use a start to initialize shortest paths
    for (int i = 0; i < columns*rows; ++i)
    {
        for (int pathIndex = 0; pathIndex < columns * rows; ++pathIndex)
        {
            GridPos start = { i / columns,i % columns };
            GridPos end = { pathIndex / columns,pathIndex % columns };
            Path& path = pathTable[i][pathIndex];
            path.paths.clear();
            path.reachable = false;
            Node* pStartNode = map+i;
            Node* pEndNode = map + pathIndex;
            memset(open, 0, MAP_AREA / 8 + 1);
            memset(closed, 0, MAP_AREA / 8 + 1);
            openList.Clear();
            pStartNode->givenCost = 0;
            pStartNode->finalCost = Octile(pStartNode->pos, pEndNode->pos);
            pStartNode->parent = nullptr;
            openList.Insert(pStartNode);
            while (openList.IsEmpty() == false)
            {
                Node* parentNode = openList.Pop();
                if (parentNode == pEndNode)
                {
                    path.reachable = true;
                    while (pEndNode != pStartNode)
                    {
                        short index = short(pEndNode - map);
                        path.paths.push_back(index);
                        pEndNode = pEndNode->parent;
                    }
                    short index = short(pEndNode-map);
                    path.paths.push_back(index);
                    break;
                }
                unsigned short index = unsigned short(parentNode - map);
                closed[index / 8] |= 1 << (index % 8);
                for (int i = 0; i < parentNode->nCount; ++i)
                {
                    unsigned char nI = parentNode->neighbours[i];
                    int neighbourIndex = index + ADJACENT[nI];
                    Node& node = map[neighbourIndex];
                    float givenCost = nI < 4 ? parentNode->givenCost + 1 : parentNode->givenCost + SQRT2;
                    byte& openBits = open[neighbourIndex / 8];
                    int bitsOffset{ 1 << (neighbourIndex % 8) };
                    if (!(openBits & bitsOffset) && !(closed[neighbourIndex / 8] & bitsOffset))
                    {
                        node.parent = parentNode;
                        openBits |= bitsOffset;
                        node.givenCost = givenCost;
                        node.finalCost = givenCost + Octile(node.pos, end);
                        openList.Insert(&node);
                    }
                    //Already on some list but the cost was higher
                    else
                    {
                        float cost = givenCost + Octile(node.pos, end);
                        if (node.finalCost > cost)
                        {
                            if (openBits & bitsOffset)
                                openList.Remove(&node);
                            else
                                openBits |= bitsOffset;
                            node.parent = parentNode;
                            node.givenCost = givenCost;
                            node.finalCost = cost;
                            openList.Insert(&node);
                        }
                    }
                }
            }
        }
    }
}



PathResult AStarPather::compute_path(PathRequest &request)
{
    /*
        weight - the heuristic weight to be applied
        newRequest - whether this is the first request for this path, should generally
            be true, unless single step is on
        smoothing - whether to apply smoothing to the path
    */
    if (speedTest)
        return SpeedTest(request);

    GridPos start = terrain->get_grid_position(request.start);
    GridPos end = terrain->get_grid_position(request.goal);

    size_t startIndex = start.row * columns + start.col;
    size_t endIndex = end.row * columns + end.col;

    Node* pStartNode = map + start.row * columns + start.col;
    Node* pEndNode = map + end.row * columns + end.col;



    if (request.settings.method == Method::FLOYD_WARSHALL)
    {
        Path& path = pathTable[startIndex][endIndex];
        if (path.reachable)
        {
            for (short index : path.paths)
            {
                request.path.push_front(terrain->get_world_position(map[index].pos));
            }
            return PathResult::COMPLETE;
        }
        return PathResult::IMPOSSIBLE;
    }

    HeuristicAlgorithm::HeuristicFunction hFunction =
        HeuristicAlgorithm::GetFunction(request.settings.heuristic);

    if (request.newRequest)
    {
        memset(open, 0, MAP_AREA/8 + 1);
        memset(closed, 0, MAP_AREA/8 + 1);
        openList.Clear();
        pStartNode->givenCost = 0;
        pStartNode->finalCost = hFunction(start, end);
        pStartNode->parent = nullptr;
        openList.Insert(pStartNode);
    }
    while (openList.IsEmpty() == false)
    {
        Node* parentNode = openList.Pop();
        if (parentNode == pEndNode)
        {
            if (request.settings.rubberBanding && request.settings.smoothing)
            {
                RubberbandAndSmoothing(pStartNode, pEndNode, request);
            }
            else if (request.settings.rubberBanding)
            {
                Rubberband(pStartNode,pEndNode,request);
            }
            else if (request.settings.smoothing)
            {
                Smooth(pStartNode, pEndNode, request);
            }
            else
            {
                while (pEndNode != pStartNode)
                {
                    request.path.push_front
                    (terrain->get_world_position(pEndNode->pos));
                    pEndNode = pEndNode->parent;
                }
                request.path.push_front
                (terrain->get_world_position(pEndNode->pos));
            }
            return PathResult::COMPLETE;
        }
        unsigned short index = unsigned short(parentNode - map);
        closed[index / 8] |= 1 << (index % 8);
        if (request.settings.debugColoring)
            terrain->set_color(parentNode->pos, Colors::Yellow);
        for (int i = 0; i < parentNode->nCount; ++i)
        {
            unsigned char nI = parentNode->neighbours[i];
            int neighbourIndex = index + ADJACENT[nI];
            Node& node = map[neighbourIndex];
            float givenCost = nI < 4? parentNode->givenCost + 1 : parentNode->givenCost + SQRT2;
            byte& openBits = open[neighbourIndex / 8];
            int bitsOffset{ 1 << (neighbourIndex % 8) };
            if (!(openBits & bitsOffset) && !(closed[neighbourIndex / 8] & bitsOffset))
            {
                node.parent = parentNode;
                openBits |= bitsOffset;
                node.givenCost = givenCost;
                node.finalCost = givenCost + hFunction(node.pos, end) * request.settings.weight;
                if (request.settings.debugColoring)
                    terrain->set_color(node.pos, Colors::Blue);
                openList.Insert(&node);
            }
            //Already on some list but the cost was higher
            else
            {
                float cost = givenCost + hFunction(node.pos, end) * request.settings.weight;
                if (node.finalCost > cost)
                {
                    if (openBits & bitsOffset)
                        openList.Remove(&node);
                    else
                        openBits |= bitsOffset;
                    node.parent = parentNode;
                    if (request.settings.debugColoring)
                        terrain->set_color(node.pos, Colors::Blue);
                    node.givenCost = givenCost;
                    node.finalCost = cost;
                    openList.Insert(&node);
                }
            }
        }
        if (request.settings.singleStep == true)
            return PathResult::PROCESSING;
    }
    return PathResult::IMPOSSIBLE;
}

void AStarPather::CallbackMapChanged()
{
    columns = terrain->get_map_width();
    rows = terrain->get_map_height();
    for (Node& node : map)
    {
        node = Node();
        int index = int((&node) - map);
        node.pos = { index / columns,index % columns };
        node.nCount = 0;
        PopulateNeighbours(&node);
    }
    ADJACENT[0] = -columns;
    ADJACENT[1] = 1;
    ADJACENT[2] = columns;
    ADJACENT[3] = -1;
    ADJACENT[4] = -columns+1;
    ADJACENT[5] = columns+1;
    ADJACENT[6] = columns-1;
    ADJACENT[7] = -columns-1;

    //FloydWarshall();
}

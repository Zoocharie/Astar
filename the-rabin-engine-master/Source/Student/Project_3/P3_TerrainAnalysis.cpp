#include <pch.h>
#include "Terrain/TerrainAnalysis.h"
#include "Terrain/MapMath.h"
#include "Agent/AStarAgent.h"
#include "Terrain/MapLayer.h"
#include "Projects/ProjectThree.h"

#include <iostream>

namespace
{
    #define SZ 0.52
    #define SQRT2 1.41421356237f

    GridPos ADJACENT[]
    {
        {1,0},  // Top
        {1,1},  // TopRight
        {0,1},  // Right
        {-1,1}, // BotRight
        {-1,0}, // Bottom
        {-1,-1},// BottomLeft
        {0,-1}, // Left
        {1,-1}, // TopLeft
    };

    float DIST[]
    {
        1,SQRT2,
        1,SQRT2,
        1,SQRT2,
        1,SQRT2
    };

    Vec2 WALLS[4]
    {
        {-SZ,SZ},  // UP LEFT
        {SZ,SZ},   // UP RIGHT
        {SZ,-SZ}, //  DOWN RIGHT
        {-SZ,-SZ}, // DOWN LEFT
    };

    static inline float Euclidean(const GridPos& start, const GridPos& end)
    {
        float xDiff = float(end.col - start.col);
        float yDiff = float(end.row - start.row);
        return sqrt(xDiff * xDiff + yDiff * yDiff);
    }
}

bool ProjectThree::implemented_fog_of_war() const // extra credit
{
    return false;
}

float distance_to_closest_wall(int row, int col)
{
    GridPos start{row,col};
    float minDist = std::numeric_limits<float>::max();
    for (int r = -1; r <= terrain->get_map_height(); ++r)
    {
        for (int c = -1; c <= terrain->get_map_width(); ++c)
        {
            if (!terrain->is_valid_grid_position(r,c) || terrain->is_wall(r, c))
            {
                if (row == r && col == c)
                    continue;
                float dist = Euclidean(start, { r,c });
                if (dist < minDist)
                {
                    if (dist == 1)
                    {
                        return 1;
                    }
                    minDist = dist;
                }
            }
        }
    }
    
    return minDist; // REPLACE THIS
}

bool is_clear_path(int row0, int col0, int row1, int col1)
{
    /*
        Two cells (row0, col0) and (row1, col1) are visible to each other if a line
        between their centerpoints doesn't intersect the four boundary lines of every
        wall cell.  You should puff out the four boundary lines by a very tiny amount
        so that a diagonal line passing by the corner will intersect it.  Make use of the
        line_intersect helper function for the intersection test and the is_wall member
        function in the global terrain to determine if a cell is a wall or not.
    */

    Vec2 lineStart{ (float)col0,(float)row0 };
    Vec2 lineEnd{ (float)col1,(float)row1 };

    int startRow = row0 < row1 ? row0 : row1;
    int startCol = col0 < col1 ? col0 : col1;

    for (int r = startRow; r < terrain->get_map_height(); ++r)
    {
        //std::cout << "FINDING CLEAR PATH" << std::endl;
        for (int c = startCol; c < terrain->get_map_width(); ++c)
        {
            if (r == startRow && c == startCol)
                continue;
            if (terrain->is_wall(r, c))
            {
                Vec2 pos = { (float)c,(float)r };
                for (int w = 0; w < 4; ++w)
                {
                    Vec2 min = pos + WALLS[w];
                    Vec2 max = pos + WALLS[(w+1)%4];
                    if (line_intersect(lineStart,lineEnd,min,max))
                        return false;
                }
            }
        }
    }

    return true;
}

void analyze_openness(MapLayer<float> &layer)
{
    /*
        Mark every cell in the given layer with the value 1 / (d * d),
        where d is the distance to the closest wall or edge.  Make use of the
        distance_to_closest_wall helper function.  Walls should not be marked.
    */
    for (int r = 0; r < terrain->get_map_height(); ++r)
    {
        for (int c = 0; c < terrain->get_map_width(); ++c)
        {
            if (terrain->is_wall(r, c))
                continue;
            float d = distance_to_closest_wall(r, c);
            layer.set_value(r,c, 1 / (d * d));
        }
    }
}

int CountVisibleTiles(int row, int col)
{
    int count = 0;
    for (int r = 0; r < terrain->get_map_height(); ++r)
    {
        //std::cout << "COUNTING VISIBLE TILES" << std::endl;
        for (int c = 0; c < terrain->get_map_width(); ++c)
        {
            if (r == row && c == col || terrain->is_wall(row,col))
                continue;
            if (is_clear_path(row, col, r, c))
                ++count;
        }
    }
    return count;
}

void analyze_visibility(MapLayer<float> &layer)
{
    /*
        Mark every cell in the given layer with the number of cells that
        are visible to it, divided by 160 (a magic number that looks good).  Make sure
        to cap the value at 1.0 as well.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    for (int r = 0; r < terrain->get_map_height(); ++r)
    {
        //std::cout << "ANALYZING VISIBILITY" << std::endl;
        for (int c = 0; c < terrain->get_map_width(); ++c)
        {
            float val = CountVisibleTiles(r, c) / 160.f;
            if (val > 1)
                val = 1;
            layer.set_value(r,c,val);
        }
    }
}

void analyze_visible_to_cell(MapLayer<float> &layer, int row, int col)
{
    /*
        For every cell in the given layer mark it with 1.0
        if it is visible to the given cell, 0.5 if it isn't visible but is next to a visible cell,
        or 0.0 otherwise.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */
    for (int r = 0; r < terrain->get_map_height(); ++r)
    {
        for (int c = 0; c < terrain->get_map_width(); ++c)
        {
            if (terrain->is_wall(r, c))
            {
                layer.set_value(r, c, 0);
                continue;
            }
            float value = 0.f;
            if (is_clear_path(row, col, r, c))
            {
                value = 1.f;
            }
            else
            {
                GridPos pos{ r,c };
                for (int i = 0; i <  8; ++i)
                {
                    GridPos neighbour = pos + ADJACENT[i];

                    if (terrain->is_valid_grid_position(neighbour) &&
                        !terrain->is_wall(neighbour) &&
                        is_clear_path(neighbour.row, neighbour.col,row,col))
                    {
                        //Corner
                        if (i % 2 == 1
                            && (terrain->is_wall(pos + ADJACENT[(i+1)%8])
                            || terrain->is_wall(pos + ADJACENT[i-1]))
                            )
                        {
                            continue;
                        }
                        value = 0.5f;
                        break;
                    }
                }
            }
            layer.set_value(r, c, value);
        }
    }
}

void analyze_agent_vision(MapLayer<float> &layer, const Agent *agent)
{
    /*
        For every cell in the given layer that is visible to the given agent,
        mark it as 1.0, otherwise don't change the cell's current value.

        You must consider the direction the agent is facing.  All of the agent data is
        in three dimensions, but to simplify you should operate in two dimensions, the XZ plane.

        Take the dot product between the view vector and the vector from the agent to the cell,
        both normalized, and compare the cosines directly instead of taking the arccosine to
        avoid introducing floating-point inaccuracy (larger cosine means smaller angle).

        Give the agent a field of view slighter larger than 180 degrees.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    Vec3 forward = agent->get_forward_vector();
    forward.y = 0;
    forward.Normalize();
    GridPos agentGridPos = terrain->get_grid_position(agent->get_position());
    Vec3 agentPos{ (float)agentGridPos.row,0,(float)agentGridPos.col };
    for (int r = 0; r < terrain->get_map_height(); ++r)
    {
        for (int c = 0; c < terrain->get_map_width(); ++c)
        {
            if (terrain->is_wall(r, c))
                continue;
            Vec3 dir = terrain->get_world_position(r, c) - agent->get_position();
            dir.y = 0;
            dir.Normalize();
            if (dir.Dot(forward) >= cos(185 / 180.f * PI / 2) && 
                is_clear_path(r,c,agentGridPos.row,agentGridPos.col))
            {
                layer.set_value(r, c, 1);
            }
        }
    }
}

void propagate_solo_occupancy(MapLayer<float> &layer, float decay, float growth)
{   
    float temp[40][40]{};

    for (int r = 0; r < terrain->get_map_height(); ++r)
    {
        for (int c = 0; c < terrain->get_map_width(); ++c)
        {
            if (terrain->is_wall(r, c))
                continue;
            GridPos pos{ r,c };
            float max = 0;
            for (int i = 0; i < 8; ++i)
            {
                GridPos neighbour = pos + ADJACENT[i];
                if (!terrain->is_valid_grid_position(neighbour) || terrain->is_wall(neighbour))
                    continue;
                if (i % 2 == 1
                    && (terrain->is_wall(pos + ADJACENT[(i + 1) % 8])
                        || terrain->is_wall(pos + ADJACENT[i - 1]))
                    )
                {
                    continue;
                }

                float val = layer.get_value(neighbour);
                float expVal = val * exp(-1 * DIST[i] * decay);
                if (max < expVal)
                {
                    max = expVal;
                }
                float influence = layer.get_value(pos);
                temp[r][c]=lerp(influence, max, growth);
            }

        }
    }

    for (int r = 0; r < terrain->get_map_height(); ++r)
    {
        for (int c = 0; c < terrain->get_map_width(); ++c)
        {
            layer.set_value(r,c,temp[r][c]);
        }
    }
}

void propagate_dual_occupancy(MapLayer<float> &layer, float decay, float growth)
{
    /*
        Similar to the solo version, but the values range from -1.0 to 1.0, instead of 0.0 to 1.0

        For every cell in the given layer:

        1) Get the value of each neighbor and apply decay factor
        2) Keep the highest ABSOLUTE value from step 1
        3) Linearly interpolate from the cell's current value to the value from step 2
           with the growing factor as a coefficient.  Make use of the lerp helper function.
        4) Store the value from step 3 in a temporary layer.
           A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

        After every cell has been processed into the temporary layer, write the temporary layer into
        the given layer;
    */

    // WRITE YOUR CODE HERE
}

void normalize_solo_occupancy(MapLayer<float> &layer)
{
    float max = std::numeric_limits<float>::min();
    for (int r = 0; r < terrain->get_map_height(); ++r)
    {
        for (int c = 0; c < terrain->get_map_width(); ++c)
        {
            if (terrain->is_wall(r, c))
                continue;
            float val = layer.get_value(r, c);
            if (val > max)
            {
                max = val;
            }
        }
    }

    for (int r = 0; r < terrain->get_map_height(); ++r)
    {
        for (int c = 0; c < terrain->get_map_width(); ++c)
        {
            if (terrain->is_wall(r, c))
                continue;
            float val = layer.get_value(r, c);
            if (val < 0)
                continue;
            layer.set_value(r,c,val/max);
        }
    }
}

void normalize_dual_occupancy(MapLayer<float> &layer)
{
    /*
        Similar to the solo version, but you need to track greatest positive value AND 
        the least (furthest from 0) negative value.

        For every cell in the given layer, if the value is currently positive divide it by the
        greatest positive value, or if the value is negative divide it by -1.0 * the least negative value
        (so that it remains a negative number).  This will keep the values in the range of [-1, 1].
    */

    // WRITE YOUR CODE HERE
}

void enemy_field_of_view(MapLayer<float> &layer, float fovAngle, float closeDistance, float occupancyValue, AStarAgent *enemy)
{
    /*
        First, clear out the old values in the map layer by setting any negative value to 0.
        Then, for every cell in the layer that is within the field of view cone, from the
        enemy agent, mark it with the occupancy value.  Take the dot product between the view
        vector and the vector from the agent to the cell, both normalized, and compare the
        cosines directly instead of taking the arccosine to avoid introducing floating-point
        inaccuracy (larger cosine means smaller angle).

        If the tile is close enough to the enemy (less than closeDistance),
        you only check if it's visible to enemy.  Make use of the is_clear_path
        helper function.  Otherwise, you must consider the direction the enemy is facing too.
        This creates a radius around the enemy that the player can be detected within, as well
        as a fov cone.
    */

    for (int r = 0; r < terrain->get_map_height(); ++r)
    {
        for (int c = 0; c < terrain->get_map_width(); ++c)
        {
            float val = layer.get_value(r, c);
            if (val < 0)
                layer.set_value(r, c, 0);
        }
    }


    Vec3 forward = enemy->get_forward_vector();
    forward.y = 0;
    forward.Normalize();
    GridPos agentGridPos = terrain->get_grid_position(enemy->get_position());
    Vec3 agentPos{ (float)agentGridPos.row,0,(float)agentGridPos.col };
    for (int r = 0; r < terrain->get_map_height(); ++r)
    {
        for (int c = 0; c < terrain->get_map_width(); ++c)
        {
            if (terrain->is_wall(r, c))
                continue;
            Vec3 dir = {r- (float)agentGridPos.row,0,c- (float)agentGridPos.col};
            Vec3 pos{ (float)r,0,(float)c };
            float dist = Vec3::Distance(pos, agentPos);
            if (dist < closeDistance)
            {
                if (is_clear_path(r, c, agentGridPos.row, agentGridPos.col))
                {
                    layer.set_value(r,c,occupancyValue);
                }
            }
            else
            {
                Vec3 dir = terrain->get_world_position(r,c) - enemy->get_position();
                dir.y = 0;
                dir.Normalize();
                if (dir.Dot(forward) >= cos((fovAngle/ 180.f) * PI / 2.f) &&
                    is_clear_path(r, c, agentGridPos.row, agentGridPos.col))
                {
                    layer.set_value(r, c, occupancyValue);
                }
            }
        }
    }
}

bool enemy_find_player(MapLayer<float> &layer, AStarAgent *enemy, Agent *player)
{
    /*
        Check if the player's current tile has a negative value, ie in the fov cone
        or within a detection radius.
    */

    const auto &playerWorldPos = player->get_position();

    const auto playerGridPos = terrain->get_grid_position(playerWorldPos);

    // verify a valid position was returned
    if (terrain->is_valid_grid_position(playerGridPos) == true)
    {
        if (layer.get_value(playerGridPos) < 0.0f)
        {
            return true;
        }
    }

    // player isn't in the detection radius or fov cone, OR somehow off the map
    return false;
}

bool enemy_seek_player(MapLayer<float> &layer, AStarAgent *enemy)
{
    /*
        Attempt to find a cell with the highest nonzero value (normalization may
        not produce exactly 1.0 due to floating point error), and then set it as
        the new target, using enemy->path_to.

        If there are multiple cells with the same highest value, then pick the
        cell closest to the enemy.

        Return whether a target cell was found.
    */

    float max = std::numeric_limits<float>::min();
    bool init = false;
    GridPos closestPos = {-1,-1};
    for (int r = 0; r < terrain->get_map_height(); ++r)
    {
        for (int c = 0; c < terrain->get_map_width(); ++c)
        {
            if (terrain->is_wall(r, c))
                continue;
            float val = layer.get_value(r, c);
            if (init && val > max - FLT_EPSILON && val < max + FLT_EPSILON)
            {
                float dist
                { Vec3::Distance(terrain->get_world_position(r,c),enemy->get_position()) };
                float prevDist
                { Vec3::Distance(terrain->get_world_position(closestPos),enemy->get_position()) };
                if (dist < prevDist)
                {
                    closestPos = { r,c };
                }
            }
            else if (val > max)
            {
                init = true;
                max = val;
                closestPos = { r,c };
            }
        }
    }
    if (terrain->is_valid_grid_position(closestPos))
    {
        enemy->path_to(terrain->get_world_position(closestPos));
        return true;
    }
    return false;
}

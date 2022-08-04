/*  
    This work is inspired by a wikipedia article "https://en.wikipedia.org/wiki/Quadtree"
    and a youtube tutorial by Aamir Mushtaq Siddiqui. However I made some modifications
    to his code so it should be more robust and can be applied also for non-symmetrical 
    cases.

    Since I am only redicovering coding in C after a long break, the code is (and will) not
    be optimal. Feel free to pull request via  "https://github.com/mrazmartin/quad_tree_c".

    Outside of the mentioned ispiration tutorial is the code written by Martin Mráz.
    https://github.com/mrazmartin

    The code is should run cross-platform, it has been tested for Linux and Windows (with MinGW).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <stdint.h>
#include <io.h> // insted of <unistd.h>

#define QT_LEAF_CAPACITY (2)


typedef struct Point {
    int x;
    int y;
} Point;

typedef struct Rectangle {
    Point* center;
    int half_width;
    int half_height;
} Rectangle;

typedef struct QuadTree {
    struct QuadTree* NW;
    struct QuadTree* NE;
    struct QuadTree* SW;
    struct QuadTree* SE;

    Point **point_array;
    Rectangle *init_screen;

} QuadTree;

int main(int argc, char const *argv[])
{
    
    return 0;
}

Point* set_up_point(int x_coord, int y_coord){
    Point *point = (Point *)malloc(sizeof(Point));
    point->x = x_coord;
    point->y = y_coord;

    return point;
} 

Rectangle* rectangle_set_up(Point *center, int half_width, int half_height){
    Rectangle* rectangle = (Rectangle*)malloc(sizeof(Rectangle));
    rectangle->center = center;
    rectangle->half_height = half_height;
    rectangle->half_width = half_width;
    return rectangle;
}

bool point_in_rectangle(Rectangle *rectangle, Point* point){
    if (point->x < rectangle->center->x - rectangle->half_width || point->x > rectangle->center->x + rectangle->half_width)
    {
        return false;
    }

    if (point->y > rectangle->center->y + rectangle->half_height || point->y < rectangle->center->y - rectangle->half_height)
    {
        return false;
    }
    
    return true;
    
}

void insert_point(Point* point, QuadTree* qt)
{

}
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
    struct QuadTree* northwest;
    struct QuadTree* northeast;
    struct QuadTree* southwest;
    struct QuadTree* southeast;

    Point **point_array;
    Rectangle *boundry_rectangle;

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

Rectangle* set_up_rectangle(Point *center, int half_width, int half_height){
    Rectangle* rectangle = (Rectangle*)malloc(sizeof(Rectangle));
    rectangle->center = center;
    rectangle->half_height = half_height;
    rectangle->half_width = half_width;
    return rectangle;
}

QuadTree* QT_subdivide(QuadTree* parent){
    float quarter_width = parent->boundry_rectangle->half_width / 2;
    float quarter_height = parent->boundry_rectangle->half_height / 2;

    Point *nw_p = Point_new(parent->boundry_rectangle->center->x - quarter_width, parent->boundry_rectangle->center->y + quarter_height);
    parent->northwest = QT_init(set_up_rectangle(nw_p, quarter_width, quarter_height));
    
    Point *ne_p = Point_new(parent->boundry_rectangle->center->x + quarter_width, parent->boundry_rectangle->center->y + quarter_height);
    parent->northeast = QT_init(set_up_rectangle(ne_p, quarter_width, quarter_height));

    Point *sw_p = Point_new(parent->boundry_rectangle->center->x - quarter_width, parent->boundry_rectangle->center->y - quarter_height);
    parent->southwest = QT_init(set_up_rectangle(sw_p, quarter_width, quarter_height));
    
    Point *se_p = Point_new(parent->boundry_rectangle->center->x + quarter_width, parent->boundry_rectangle->center->y - quarter_height);
    parent->southeast = QT_init(set_up_rectangle(se_p, quarter_width, quarter_height));
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


QuadTree* QT_init(Rectangle* rectangle){
    QuadTree* qt = (QuadTree*)malloc(sizeof(QuadTree));
    qt->northeast = NULL;
    qt->northwest = NULL;
    qt->southeast = NULL;
    qt->southwest = NULL;
    qt->boundry_rectangle = rectangle;

    qt->point_array = (Point**)malloc(sizeof(Point*)*QT_LEAF_CAPACITY);     // maximum of points stored within a rectangle
    
    for (size_t i = 0; i < QT_LEAF_CAPACITY; i++)
    {
        qt->point_array[i]=NULL;
    }
    return qt;
}

void insert_point(Point* point, QuadTree* qt)
{

}
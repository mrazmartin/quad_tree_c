/*  
    This work is inspired by a wikipedia article "https://en.wikipedia.org/wiki/Quadtree"
    and a youtube tutorial by Aamir Mushtaq Siddiqui. However I made some modifications
    to his code so it should be more robust and can be applied also for non-symmetrical 
    cases.

    Since I am only redicovering coding in C after a long break, the code is (and will) not
    be optimal. Feel free to pull request via  "https://github.com/mrazmartin/quad_tree_c".

    Outside of the mentioned ispiration tutorial is the code written by Martin Mráz
    https://github.com/mrazmartin

    The code should run cross-platform, it has been tested for Linux and Windows (with MinGW).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <stdint.h>
#include <io.h> // insted of <unistd.h>
#include <time.h>

#define QT_LEAF_CAPACITY (2)

typedef struct Point {
    float x;
    float y;
} Point;

typedef struct QT_rectangle {
    Point* center;
    float half_width;
    float half_height;
} QT_rectangle;

typedef struct QuadTree {
    struct QuadTree* northwest;
    struct QuadTree* northeast;
    struct QuadTree* southwest;
    struct QuadTree* southeast;

    bool subdivided;

    Point **point_array;
    QT_rectangle *boundry_rectangle;

} QuadTree;

Point* set_up_point(float x_coord, float y_coord){
    Point *point = (Point *)malloc(sizeof(Point));
    point->x = x_coord;
    point->y = y_coord;

    return point;
} 

QT_rectangle* set_up_rectangle(Point *center, float half_width, float half_height){
    QT_rectangle* rectangle = (QT_rectangle*)malloc(sizeof(QT_rectangle));
    rectangle->center = center;
    rectangle->half_height = half_height;
    rectangle->half_width = half_width;
    return rectangle;
}

 
QuadTree* QT_init(QT_rectangle* rectangle){
    QuadTree* qt = (QuadTree*)malloc(sizeof(QuadTree));
    qt->northeast = NULL;
    qt->northwest = NULL;
    qt->southeast = NULL;
    qt->southwest = NULL;
    qt->boundry_rectangle = rectangle;
    qt->subdivided=false;

    qt->point_array = (Point**)malloc(sizeof(Point*)*QT_LEAF_CAPACITY);     // maximum of points stored within a rectangle
    
    for (size_t i = 0; i < QT_LEAF_CAPACITY; i++)
    {
        qt->point_array[i]=NULL;
    }
    return qt;
}

QuadTree* QT_subdivide(QuadTree* parent){
    float quarter_width = parent->boundry_rectangle->half_width / 2;            // TODO: resolve cases of even division (a point exactly on the boundry may cause trouble)
    float quarter_height = parent->boundry_rectangle->half_height / 2;

    Point *nw_p = set_up_point(parent->boundry_rectangle->center->x - quarter_width, parent->boundry_rectangle->center->y + quarter_height);
    parent->northwest = QT_init(set_up_rectangle(nw_p, quarter_width, quarter_height));
    
    Point *ne_p = set_up_point(parent->boundry_rectangle->center->x + quarter_width, parent->boundry_rectangle->center->y + quarter_height);
    parent->northeast = QT_init(set_up_rectangle(ne_p, quarter_width, quarter_height));

    Point *sw_p = set_up_point(parent->boundry_rectangle->center->x - quarter_width, parent->boundry_rectangle->center->y - quarter_height);
    parent->southwest = QT_init(set_up_rectangle(sw_p, quarter_width, quarter_height));
    
    Point *se_p = set_up_point(parent->boundry_rectangle->center->x + quarter_width, parent->boundry_rectangle->center->y - quarter_height);
    parent->southeast = QT_init(set_up_rectangle(se_p, quarter_width, quarter_height));

    parent->subdivided = true;
}

bool point_in_rectangle(QT_rectangle *rectangle, Point* point){
    
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

bool check_rectangle_intersection(QT_rectangle* main_rect, QT_rectangle* other_rect){
    
    //check horizontal
    if (main_rect->center->x + main_rect->half_width > other_rect->center->x - other_rect->half_width)
    {
        return true;
    }
    
    if (main_rect->center->x - main_rect->half_width < other_rect->center->x + other_rect->half_width)
    {
        return true;
    }
    
    // check vertical

    if (main_rect->center->y + main_rect->half_height < other_rect->center->y + other_rect->half_height)
    {
        return true;
    }
    if (main_rect->center->y - main_rect->half_height > other_rect->center->y - other_rect->half_height)
    {
        return true;
    }
    
    return false;
    
}

size_t qt_node_point_size(QuadTree* qt){
    size_t point_size;

    for(point_size=0; point_size<QT_LEAF_CAPACITY;point_size++){
        if (qt->point_array[point_size]==NULL)
        {
            return point_size;
        }
    }

    return point_size;
}

void print_point(Point* point){
    printf("Point: [%f ; %f]\n", point->x, point->y);
}

bool QT_insert_point(Point* point, QuadTree* qt)
{
    
    if (!point_in_rectangle(qt->boundry_rectangle, point))
    {

        return false; // if the point is not present in the given rectangle, there is no reason to insert it there (the function serves to differentiate between the 4 sections of a qt node)
    }

    size_t num_points = qt_node_point_size(qt);

    if (num_points < QT_LEAF_CAPACITY && qt->subdivided)
    {
        qt->point_array[num_points] = point;
        printf("Inserted ");
        print_point(point);
        printf("\n");
        return true;
    }
    
    if (!qt->subdivided)
    {
        printf("%sDIVIDING%s tree branch\n", "\x1B[33m", "\x1B[0m");
        QT_subdivide(qt);
    }
    
    if (QT_insert_point(point, qt->northwest));
    {
        printf("Inserted NW ");
        print_point(point);
        return true;
    }
    if (QT_insert_point(point, qt->northeast));
    {
        printf("Inserted NE ");
        print_point(point);
        return true;
    }
    if (QT_insert_point(point, qt->southwest));
    {
        printf("Inserted SW ");
        print_point(point);
        return true;
    }
    if (QT_insert_point(point, qt->southeast));
    {
        printf("Inserted SE ");
        print_point(point);
        return true;
    }

    return false;   // TODO check if there is another option available -> if not: stderr
}

Point* get_center(float width, float height){       // somewhat duplicate to the set_up_point function

    Point* point = (Point*)malloc(sizeof(Point));
    point->x = width/2;
    point->y = height/2;
    
    return point;
}

#define X 200
#define Y 200

int main(int argc, char const *argv[])
{
    float width, height;
    width = X;
    height = Y;

    Point* center = get_center(width, height);

    printf("The 'point' has coords: [%f ; %f]\n\n", center->x, center->y);

    QT_rectangle* rectangle = set_up_rectangle(center, width/2, height/2);


    int upper = 200, lower = 0;
    srand(time(0));

    QuadTree* qt = QT_init(rectangle);

    for (size_t i = 0; i < 10; i++)
    {

        int x = (rand() % (upper - lower + 1)) + lower;
        int y = (rand() % (upper - lower + 1)) + lower;

        Point* point = set_up_point(x,y);
        print_point(point);
        if(!QT_insert_point(point, qt)){
            printf("\n%sFAILED%s TO INSERT POINT\n\n", "\x1B[31m", "\x1b[0m");
        }
    }
}
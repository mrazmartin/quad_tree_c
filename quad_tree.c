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

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct Rectangle {
    int center_x;
    int center_y;
    int width;
    int height;
} Rectangle;

typedef struct QuadTree {
    struct QuadTree* NW;
    struct QuadTree* NE;
    struct QuadTree* SW;
    struct QuadTree* SE;

    Point **point_array;
    Rectangle *init_screen;
    
} QuadTree;
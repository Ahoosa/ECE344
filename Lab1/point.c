#include <assert.h>
#include "common.h"
#include "point.h"
#include <math.h>
#include <stdio.h>

/* update *p by increasing p->x by x and p->y by y */
void point_translate(struct point *p, double x, double y){
   p->x= p->x+x;
   p->y= p->y+y;
}

/* return the cartesian distance between p1 and p2 */
double point_distance(const struct point *p1, const struct point *p2){
    return sqrt(pow((p2->x-p1->x),2 )+ pow((p2->y-p1->y),2));
}

/* this function compares the Euclidean lengths of p1 and p2. The Euclidean
 * length of a point is the distance of the point from the origin (0, 0). The
 * function should return -1, 0, or 1, depending on whether p1 has smaller
 * length, equal length, or larger length, than p2. */
int point_compare(const struct point *p1, const struct point *p2){
    double length1=sqrt(pow((p1->x),2)+pow((p1->y),2));
    double length2=sqrt(pow((p2->x),2)+pow((p2->y),2));
    if(length1>length2) return 1;
    else if(length1<length2) return -1;
    else return 0;
}




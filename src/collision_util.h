#ifndef COLLISION_UTIL
#define COLLISION_UTIL

#include "math.h"

// Vector3 coordinate type for 3D collision detection
typedef struct Vector3
{
    int x;
    int y;
    int z;
} Vector3;

#define v3sub(a,b) {(a.x-b.x,a.y-b.y,a.z-b.z)}
#define v3dist(a) sqrt((a.x*a.x)+(a.y*a.y)+(a.z*a.z))

// Vector2 coordinate type for 2D collision detection
typedef struct Vector2
{
    int x;
    int y;
} Vector2;

#define v2sub(a,b) {a.x-b.x,a.y-b.y}
#define v2dist(a) sqrt((a.x*a.x)+(a.y*a.y))
#define v2eq(a,b) (a.x==b.x||a.y==b.y)

/*
- Calculates where two cubiods (a and b) intersect
- a1 and a2 represent two opposing vertices of the first cubiod,
- b1 and b2 represent two opposing vertices of the second cubiod 
- return 1 if there is an intersection, 0 if not
*/
int collision_Cubiod(Vector3 a1,Vector3 a2, Vector3 b1,Vector3 b2);
/*
- Calculates where two spheres (a and b) intersect
- a1 is the centre of the first sphere, ar the radius of the first sphere
- b1 is the centre of the second sphere, br the radius of the second sphere 
- return 1 if there is an intersection, 0 if not
*/
int collision_Sphere(Vector3 a,int ar, Vector3 b, int br);
/*
- Calculates where a sphere and cubiod intersect
- a1: centre of the sphere, ar: radius of the sphere
- b1, b2: two opposing verices of the cubiod
- return 1 if there is an intersection, 0 if not
*/
int collision_Cubiod_Sphere(Vector3 a1,Vector3 ar,Vector3 b1, Vector3 b2);

int collision_Rect(Vector2 a1, Vector2 a2, Vector2 b1, Vector2 b2);

int collision_Circle(Vector2 a1, int ar, Vector2 b1, int br);

int collision_Rect_Circle(Vector2 a1, Vector2 a2, Vector2 b1, int br);

#endif
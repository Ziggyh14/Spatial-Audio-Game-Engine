#include "collision_util.h"

int collision_Cubiod(Vector3 a1,Vector3 a2, Vector3 b1,Vector3 b2);

int collision_Sphere(Vector3 a,int ar, Vector3 b, int br){

    /**
     * Calculate difference vector
     * Calculate distance between both centres
     * if distance > sum of radiuses then then do not intersect
    */

    Vector3 vdiff = v3sub(a,b);
    float d = v3dist(vdiff);
    if(d > (float) (ar+br))
        return 0;
    return 1;
}

int collision_Cubiod_Sphere(Vector3 a1,Vector3 ar,Vector3 b1, Vector3 b2);

int collision_Rect(Vector2 a1, Vector2 a2, Vector2 b1, Vector2 b2){

    if(v2eq(a1,a2) || v2eq(b1,b2))
        return 0;

    if(a1.x > b2.x || b1.x > a2.x)
        return 0;
    
    if(a2.y > b1.y || b2.y > a1.y)
        return 0;
    
    return 1;
}

int collision_Circle(Vector2 a, int ar, Vector2 b, int br){

    Vector2 vdiff = v2sub(a,b);
    float d = v2dist(vdiff);
    if(d > (float) (ar+br))
        return 0;
    return 1;
}

int collision_Rect_Circle(Vector2 a1, Vector2 a2, Vector2 b1, int br);
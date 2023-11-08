#include "collision_util.h"

int collision_Cubiod(Vector3 a1,Vector3 a2, Vector3 b1,Vector3 b2);

int collision_Sphere(Vector3 a1,int ar, Vector3 b1, int br);

int collision_Cubiod_Sphere(Vector3 a1,Vector3 ar,Vector3 b1, Vector3 b2);

int collision_Rect(Vector2 a1, Vector2 a2, Vector2 b1, Vector2 b2);

int collision_Circle(Vector2 a1, int ar, Vector2 b1, int br);

int collision_Rect_Circle(Vector2 a1, Vector2 a2, Vector2 b1, int br);
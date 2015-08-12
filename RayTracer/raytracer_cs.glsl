#version 430 core

layout(binding = 0, rgba32f) uniform image2D framebuffer;

uniform vec3 eye;
uniform vec3 ray00;
uniform vec3 ray01;
uniform vec3 ray10;
uniform vec3 ray11;

const float NO_INTERSECT = 1.0 / 0.0;
const int TYPE_NULL = -1;
const int TYPE_AABB = 0;
const int TYPE_SPHERE = 1;

//Materials
struct Materials
{
  vec4 color;
};

const uint n_materials = 4;

const Materials MaterialsList[n_materials] = 
{
  {vec4(1.0, 0.0, 0.0, 1.0)},
  {vec4(1.0, 1.0, 0.0, 1.0)},
  {vec4(1.0, 0.0, 1.0, 1.0)},
  {vec4(0.0, 1.0, 1.0, 1.0)},
};

//Geomertries
struct AABB 
{
  vec3 min;
  vec3 max;
  uint materials;
};

struct OBB
{
  vec3 v0;
  vec3 v1;
  vec3 v2;
  vec3 u;
  uint materials;
};

struct Sphere
{
  vec3  center;
  float radius;
  uint materials;
};

struct Capsule
{
  uint materials;
  vec3  origin;
  vec3  direction;
  float radius;
};

struct Cylinder
{
  uint materials;
  vec3  origin;
  vec3  direction;
  float radius;
};

struct Torus
{
  uint materials;
  vec3  center;
  vec3  axis;
  float R;
  float r;
};

struct ConeSegment
{
  uint materials;
  vec3  origin;
  vec3  direction;
  float r0;
  float r1;
};

#define MAX_SCENE_BOUNDS 100.0
#define NUM_BOXES   2
#define NUM_SPHERES 1

const float dx = 10.0;
const float dy = 10.0;
const float dz = 10.0;

const AABB boxes[] = 
{
  /* y */
  {vec3(-2.5, -2.5 + dy, -2.5), vec3(2.5, 2.5 + dy, 2.5), 1},
  /* z */
  {vec3(-3.5, -3.5, -3.5 + dz), vec3(3.5, 3.5, 3.5 + dz), 3},
};

const Sphere spheres[] = 
{
  {vec3(-1.5 + dx, -1.5, -1.5), 2.0, 2}
};

struct HitInfo 
{
  int type;
  float t;
  int  index;
};

struct Ray
{
  vec3 P;
  vec3 V;
};

float IntersectSphereFromOutside(const Ray ray, const Sphere sphere)
{
  vec3  P = ray.P - sphere.center;
  float b = 2.0 * dot(P, ray.V);
  float a = dot(ray.V, ray.V);
  float c = dot(P, P) - sphere.radius * sphere.radius;
  float discriment = b * b - 4.0 * a * c;

  // Ray does not intersect sphere
  if (discriment <= 0.0)
  {
    return NO_INTERSECT;
  }

  float numerator = -b - sqrt(discriment);

  // Sphere behind ray origin
  if (numerator < 0.0)
  {
    return NO_INTERSECT;
  }

  return numerator / (2.0 * a);
}

float IntersectSphereFromInside(const Ray ray, const Sphere sphere)
{
  vec3  P = ray.P - sphere.center;
  float b = 2.0 * dot(P, ray.V);
  float a = dot(ray.V, ray.V);
  float c = dot(P, P) - sphere.radius * sphere.radius;
  float discriment = b * b - 4.0 * a * c;

  // Ray does not intersect sphere. ERROR: Ray SHOULD intersect sphere!
  if (discriment <= 0.0)
  {
    return NO_INTERSECT;
  }

  float numerator = -b + sqrt(discriment);

  // Sphere behind ray origin. ERROR: Ray SHOULD intersect sphere ahead!
  if (numerator < 0.0)
  {
    return NO_INTERSECT;
  }

  return numerator / (2.0 * a);
}

float IntersectSphere(const Ray ray, const Sphere sphere)
{
  vec3 P_C = ray.P - sphere.center;
  if (dot(P_C, P_C) <= sphere.radius * sphere.radius)
  {
    return IntersectSphereFromInside(ray, sphere);
  }
  return IntersectSphereFromOutside(ray, sphere);
}

float IntersectAABBFromOutside(const Ray ray, const AABB b) 
{
  vec3 tMin = (b.min - ray.P) / ray.V;
  vec3 tMax = (b.max - ray.P) / ray.V;
  vec3 t1 = min(tMin, tMax);
  vec3 t2 = max(tMin, tMax);
  float tNear = max(max(t1.x, t1.y), t1.z);
  float tFar = min(min(t2.x, t2.y), t2.z);
  
  if (tNear > 0.0 && tNear < tFar)
  {
    return tNear;
  }
  return NO_INTERSECT;
}

float IntersectAABBFromInside(const Ray ray, const AABB b) 
{
  vec3 tMin = (b.min - ray.P) / ray.V;
  vec3 tMax = (b.max - ray.P) / ray.V;
  vec3 t1 = min(tMin, tMax);
  vec3 t2 = max(tMin, tMax);
  float tNear = max(max(t1.x, t1.y), t1.z);
  float tFar = min(min(t2.x, t2.y), t2.z);
  
  if (tNear < tFar)
  {
    return tNear;
  }
  return NO_INTERSECT;
}

float IntersectAABB(const Ray ray, const AABB b) 
{
  if (ray.P.x > b.min.x
    && ray.P.y > b.min.y
    && ray.P.z > b.min.z
    && ray.P.x < b.max.x
    && ray.P.y < b.max.y
    && ray.P.z < b.max.z)
    {
      return IntersectAABBFromInside(ray, b);
    }
    return IntersectAABBFromOutside(ray, b);
}

void IntersectAABBs(const Ray ray, inout HitInfo info) 
{
  for (int i = 0; i < NUM_BOXES; i++) 
  {
    float t = IntersectAABB(ray, boxes[i]);
    if (t < info.t) 
    {
      info.type = TYPE_AABB;
      info.t = t;
      info.index = i;
    }
  }
}

void IntersectSpheres(const Ray ray, inout HitInfo info) 
{
  for (int i = 0; i < NUM_SPHERES; i++) 
  {
    float t = IntersectSphere(ray, spheres[i]);
    if (t < info.t) 
    {
      info.type = TYPE_SPHERE;
      info.t = t;
      info.index = i;
    }
  }
}

vec4 trace(const Ray ray) 
{
  HitInfo info;
  info.type = TYPE_NULL;
  info.t = MAX_SCENE_BOUNDS;
  
  IntersectSpheres(ray, info);
  IntersectAABBs(ray, info);

  if (info.type == TYPE_AABB)
  {
    return MaterialsList[boxes[info.index].materials].color;
  }
  else if (info.type == TYPE_SPHERE)
  {
    return MaterialsList[spheres[info.index].materials].color;
  }

  return vec4(0.0, 0.0, 0.0, 1.0);
}


layout (local_size_x = 16, local_size_y = 8) in;
void main(void) {
  ivec2 pix = ivec2(gl_GlobalInvocationID.xy);
  ivec2 size = imageSize(framebuffer);
  if (pix.x >= size.x || pix.y >= size.y) {
    return;
  }
  vec2 pos = vec2(pix) / vec2(size.x - 1, size.y - 1);
  pos = 1 - pos;
  vec3 dir = mix(mix(ray00, ray01, pos.y), mix(ray10, ray11, pos.y), pos.x);
  Ray ray;
  ray.P = eye;
  ray.V = dir;
  vec4 color = trace(ray);
  imageStore(framebuffer, pix, color);
}

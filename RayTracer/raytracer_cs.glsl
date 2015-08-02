#version 430 core

layout(binding = 0, rgba32f) uniform image2D framebuffer;

uniform vec3 eye;
uniform vec3 ray00;
uniform vec3 ray01;
uniform vec3 ray10;
uniform vec3 ray11;

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
  uint materials;
  vec3 v0;
  vec3 v1;
  vec3 v2;
  vec3 u;
};

struct Sphere
{
  uint materials;
  vec3  center;
  float radius;
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
#define NUM_BOXES 4

const float dx = 10.0;
const float dy = 10.0;
const float dz = 10.0;

const AABB boxes[] = 
{
  /* Origin */
  {vec3(-0.5, -0.5, -0.5), vec3(0.5, 0.5, 0.5), 0},
  /* x */
  {vec3(-1.5 + dx, -1.5, -1.5), vec3(1.5 + dx, 1.5, 1.5), 1},
  /* y */
  {vec3(-2.5, -2.5 + dy, -2.5), vec3(2.5, 2.5 + dy, 2.5), 2},
  /* z */
  {vec3(-3.5, -3.5, -3.5 + dz), vec3(3.5, 3.5, 3.5 + dz), 3},
};

struct hitinfo 
{
  vec2 lambda;
  int bi;
  vec4 color;
};

vec2 intersectBox(vec3 origin, vec3 dir, const AABB b) 
{
  vec3 tMin = (b.min - origin) / dir;
  vec3 tMax = (b.max - origin) / dir;
  vec3 t1 = min(tMin, tMax);
  vec3 t2 = max(tMin, tMax);
  float tNear = max(max(t1.x, t1.y), t1.z);
  float tFar = min(min(t2.x, t2.y), t2.z);
  return vec2(tNear, tFar);
}

bool intersectBoxes(vec3 origin, vec3 dir, out hitinfo info) 
{
  float smallest = MAX_SCENE_BOUNDS;
  bool found = false;
  for (int i = 0; i < NUM_BOXES; i++) 
  {
    vec2 lambda = intersectBox(origin, dir, boxes[i]);
    if (lambda.x > 0.0 && lambda.x < lambda.y && lambda.x < smallest) {
      info.lambda = lambda;
      info.bi = i;
      info.color = MaterialsList[boxes[i].materials].color;
      smallest = lambda.x;
      found = true;
    }
  }
  return found;
}

vec4 trace(vec3 origin, vec3 dir) 
{
  hitinfo i;
  if (intersectBoxes(origin, dir, i)) 
  {
    return i.color;
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
  vec4 color = trace(eye, dir);
  //imageStore(framebuffer, pix, vec4(dir.y, 0.0, 0.0, 1.0));
  imageStore(framebuffer, pix, color);
}

#ifndef SCENE_H
#define SCENE_H

#include <stdint.h>
#include <stdlib.h>

#include "RayTracerConfig.h"
#include "VQS.h"
//#include "BasisR3.h"

typedef Dg::VQS<real>     vqs;
typedef Dg::Vector4<real> vec4;

struct IntersectionData
{

};

struct Materials
{
  uint32_t color;
};

struct Sphere
{
  vec4 center;
  real radius;
  Materials materials;
};

struct OBB
{
  vec4 center;
  //BasisR3 basis;
  real x, y, z;
  Materials materials;
};

struct Torus
{
  vec4 center;
  vec4 axis;
  real radius_circle;
  real radius_thick;
  Materials materials;
};

struct Mesh
{
  vec4 * vertices;
  int nVertices;
  int * facets[3];
  Materials materials;
};

struct Ray
{
  vec4 origin;
  vec4 direction;
};

template<typename T>
class qArray
{
public:

  qArray() : size(0), data(nullptr) {}

  ~qArray() { free(data); }

  void Resize(unsigned a_size) 
  {
    size = a_size;

    if (a_size == 0)
    {
      free(data);
      data = nullptr;
    }
    else
    {
      data = realloc(data, size * sizeof(T));
    }
  }

public:

  unsigned size;
  T * data;

};


class Scene
{
public:

private:

  qArray<Sphere> m_spheres;

  vqs m_camera;

};

#endif
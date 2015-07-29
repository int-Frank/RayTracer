#ifndef CAMERA_H
#define CAMERA_H

#include "RayTracerConfig.h"
#include "Matrix44.h"
#include "Vector4.h"

typedef Dg::Vector4<float> vec4;
typedef Dg::Matrix44<float> mat4;

class Camera
{
public:

  Camera() : m_roll(0.0f)
           , m_pitch(0.0f)
           , m_yaw(0.0f)
           , m_ar(1.0f)
           , m_projDist(1.0f){}

  void SetScreen(float a_ar, float a_projDist);

  void SetYPR(float a_yaw, 
              float a_pitch,
              float a_roll);

  void UpdateYPR(float a_yaw,
                 float a_pitch,
                 float a_roll);

  void MoveForward(float a_dx);
  void MoveLeft(float a_dx);
  void MoveUp(float a_dx);
  void MoveWorldUp(float a_dx);

  void GetCornerRays(vec4 & a_ray00,
                     vec4 & a_ray01, 
                     vec4 & a_ray10,
                     vec4 & a_ray11,
                     vec4 & a_origin);

private:

  void GenerateMatrix();

private:

  float   m_ar; // length / height
  float   m_projDist;

  float   m_roll;
  float   m_pitch;
  float   m_yaw;

  mat4    m_matrix;
};

#endif